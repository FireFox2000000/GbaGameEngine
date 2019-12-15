using System;
using System.Text;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Collections.Generic;

namespace GbaConversionTools.Tools
{
    class SpriteConverter
    {
        public struct SliceCoordinate
        {
            public int x, y, width, height;
        }

        const int c_arrayNewlineCount = 10;
        const int c_TILEWIDTH = 8;
        const int c_TileHEIGHT = 8;

        const string TAB_CHAR = Defines.TAB_CHAR;
        const string namespaceTabs = TAB_CHAR;
        const string STR_U8 = Defines.STR_U8;
        const string STR_U16 = Defines.STR_U16;
        const string STR_U32 = Defines.STR_U32;
        const string VARPREFIXES = "extern " + Defines.STR_EWRAM_DATA + " const ";
        const string NAMESPACE_FORMAT                   = "namespace {0} \n{{\n";
        const string VAR_HEADER_PALLETLENGTH_FORMAT     = namespaceTabs + VARPREFIXES + STR_U8 + " paletteLength = {0};\n";
        const string VAR_HEADER_DATALENGTH_FORMAT       = namespaceTabs + VARPREFIXES + STR_U32 + " dataLength = {0};\n\n";
        const string VAR_PALLET                         = namespaceTabs + VARPREFIXES + STR_U16 + " palette[] = \n";
        const string VAR_SPRITECOUNT_FORMAT             = namespaceTabs + VARPREFIXES + STR_U32 + " spriteCount = {0};\n";
        const string VAR_WIDTHMAP                       = namespaceTabs + VARPREFIXES + STR_U8 + " widthMap[] = \n";
        const string VAR_HEIGHTMAP                      = namespaceTabs + VARPREFIXES + STR_U8 + " heightMap[] = \n";
        const string VAR_OFFSETS                        = namespaceTabs + VARPREFIXES + STR_U32 + " offsets[] = \n";
        const string VAR_DATA                           = namespaceTabs + VARPREFIXES + STR_U16 + " data[] = \n";

        public void Convert(string inputPath, string outputPath, Bitmap bitmap, SliceCoordinate[] sliceCoordinates)
        {
            StringBuilder sb = new StringBuilder();
            Size size = bitmap.Size;
            if (size.Width % c_TILEWIDTH != 0 || size.Height % c_TileHEIGHT != 0)
            {
                throw new Exception("Size not compatible with GBA tiles. Width and height of pixels must be multiples of 8.");
            }

            Console.WriteLine("Processing colour palette");
            Color[] preProcessedPalette = GeneratePreprocessedPalette(bitmap);

            // Validate pallet length
            {
                if (preProcessedPalette.Length > 256)
                {
                    throw new Exception("Palette length out of range for the GBA");
                }

                // Todo, currently not supported
                if (preProcessedPalette.Length > 16)
                {
                    throw new Exception("Palette length out of range for 4bbp format");
                }
            }

            string namespaceName = Path.GetFileName(Path.GetFileNameWithoutExtension(inputPath));
            
            sb.AppendFormat(NAMESPACE_FORMAT, namespaceName);

            sb.AppendFormat(VAR_SPRITECOUNT_FORMAT, sliceCoordinates.Length);
            WriteHeader(bitmap, preProcessedPalette, sb);
            WritePalette(preProcessedPalette, sb);

            // Write width
            {
                sb.Append(VAR_WIDTHMAP);
                sb.Append(namespaceTabs + "{\n");
                sb.Append(namespaceTabs + TAB_CHAR);

                for (int i = 0; i < sliceCoordinates.Length; ++i)
                {
                    int spriteWidth = sliceCoordinates[i].width;
                    sb.AppendFormat("{0}, ", spriteWidth);
                    if ((i + 1) % c_arrayNewlineCount == 0)
                        sb.AppendFormat("\n" + namespaceTabs + TAB_CHAR);
                }

                sb.Append("\n");
                sb.Append(namespaceTabs + "};\n\n");
            }

            // Write height
            {
                sb.Append(VAR_HEIGHTMAP);
                sb.Append(namespaceTabs + "{\n");
                sb.Append(namespaceTabs + TAB_CHAR);

                for (int i = 0; i < sliceCoordinates.Length; ++i)
                {
                    int spriteHeight = sliceCoordinates[i].height;   // Todo
                    sb.AppendFormat("{0}, ", spriteHeight);
                    if ((i + 1) % c_arrayNewlineCount == 0)
                        sb.AppendFormat("\n" + namespaceTabs + TAB_CHAR);
                }

                sb.Append("\n");
                sb.Append(namespaceTabs + "};\n\n");
            }

            List<int> dataOffsets = new List<int>();
            List<StringBuilder> spriteData = new List<StringBuilder>();
            dataOffsets.Add(0);

            // Collect data and add offsets
            {
                Console.WriteLine("Processing sprite data");

                for (int i = 0; i < sliceCoordinates.Length; ++i)
                {
                    StringBuilder dataSb = new StringBuilder();
                    SliceCoordinate slice = sliceCoordinates[i];
                    int spriteWidth = slice.width, spriteHeight = slice.height;    // Todo, determine through some kind of config file
                    int dataCount = WriteSpriteData(dataSb, bitmap, preProcessedPalette, spriteWidth, spriteHeight, slice.x, slice.y);

                    spriteData.Add(dataSb);

                    // Add offsets
                    if (i < sliceCoordinates.Length - 1)
                        dataOffsets.Add(dataOffsets[i] + dataCount);
                }
                
            }

            // Write offsets
            {
                sb.Append(VAR_OFFSETS);
                sb.Append(namespaceTabs + "{\n");
                for (int i = 0; i < dataOffsets.Count; i++)
                {
                    const string tabs = namespaceTabs + TAB_CHAR;
                    sb.AppendFormat(tabs + "{0}, \n", dataOffsets[i]);
                }

                sb.Append(namespaceTabs + "};\n\n");
            }

            // Write data
            {
                sb.Append(VAR_DATA);
                sb.Append(namespaceTabs + "{\n");

                foreach(StringBuilder dataSB in spriteData)
                {
                    sb.Append(dataSB.ToString());
                }

                sb.Append(namespaceTabs + "};\n\n");
            }

            sb.Append("}\n");

            Console.WriteLine("Sprite \"" + outputPath + "\" successfully converted");
            File.WriteAllText(outputPath, sb.ToString());
        }

        Color[] GeneratePreprocessedPalette(Bitmap bitmap)
        {
            List<Color> palette = new List<Color>();
            Color transparencyColour = Color.FromArgb(0);
            palette.Add(transparencyColour);     // Always make transparent the first index

            for (int y = 0; y < bitmap.Height; ++y)
            {
                for (int x = 0; x < bitmap.Width; ++x)
                {
                    Color color = bitmap.GetPixel(x, y);
                    
                    if (color.A != 0 && color.A != 255)
                    {
                        Console.WriteLine(string.Format("Warning: Found pixel with invalid alpha value ({2}) at position ({0}, {1}). Transparency is not allowed.", x, y, color.A));
                    }

                    bool foundInPalette = false;

                    foreach (Color palColor in palette)
                    {
                        // Don't need to compare alpha
                        if (color == palColor)
                        {
                            foundInPalette = true;
                            break;
                        }
                    }

                    if (!foundInPalette)
                        palette.Add(color);
                }
            }

            return palette.ToArray();
        }

        void WriteHeader(Bitmap bitmap, Color[] palette, StringBuilder sb)
        {
            Size size = bitmap.Size;

            //sb.AppendFormat(namespaceTabs + "extern const " + c_u16 + " width = {0}, height = {1}; \n", size.Width, size.Height);
            sb.AppendFormat(VAR_HEADER_PALLETLENGTH_FORMAT, palette.Length);
            sb.AppendFormat(VAR_HEADER_DATALENGTH_FORMAT, size.Width * size.Height / 4);
        }

        void WritePalette(Color[] palette, StringBuilder sb)
        {
            sb.Append(VAR_PALLET);
            sb.Append(namespaceTabs + "{\n\t" + namespaceTabs);
            for (int i = 0; i < palette.Length; ++i)
            {
                Color color = palette[i];
                UInt16 rbgColor = (UInt16)(ScaleToRgb16(color.R) + (ScaleToRgb16(color.G) << 5) + (ScaleToRgb16(color.B) << 10));

                sb.AppendFormat("0x{0:X4}, ", rbgColor);
                if ((i + 1) % c_arrayNewlineCount == 0)
                {
                    sb.Append("\n\t" + namespaceTabs);
                }
            }
            sb.Append("\n" + namespaceTabs + "};\n\n");
        }

        int ColorToPaletteIndex(Color[] palette, Color color)
        {
            for (int i = 0; i < palette.Length; ++i)
            {
                if (color == palette[i])
                    return i;
            }

            throw new Exception("Colour not found in palette");
        }

        byte ScaleToRgb16(byte colorChannelValue)
        {
            return (byte)(Math.Round(colorChannelValue / 255.0f * 31.0f));
        }

        void WriteSprite(StringBuilder sbOutput, Bitmap bitmap, Color[] palette, int width, int height, int startOffsetX, int startOffsetY)
        {
            const string tabs = namespaceTabs + TAB_CHAR;

            sbOutput.Append(tabs + "{\n\t" + tabs);
            WriteSpriteData(sbOutput, bitmap, palette, width, height, startOffsetX, startOffsetY);
            sbOutput.Append("\n" + tabs + "},\n");
        }

        int WriteSpriteData(StringBuilder sbOutput, Bitmap bitmap, Color[] palette, int width, int height, int xPos, int yPos)
        {
            const string tabs = namespaceTabs;

            int tilesWide = width / c_TILEWIDTH;
            int tilesTall = height / c_TileHEIGHT;
            int totalTiles = (width * height) / (c_TILEWIDTH * c_TileHEIGHT);
            int hexCount = 0;

            sbOutput.Append(namespaceTabs + TAB_CHAR);

            for (int tileY = 0; tileY < tilesTall; ++tileY)
            {
                for (int tileX = 0; tileX < tilesWide; ++tileX)
                {
                    int tileXOffset = xPos + tileX * c_TILEWIDTH;
                    int tileYOffset = yPos + tileY * c_TileHEIGHT;

                    for (int y = 0; y < c_TileHEIGHT; ++y)
                    {
                        for (int x = 0; x < c_TILEWIDTH; x += 4)
                        {
                            UInt16 hexNum = 0;
                            for (int i = 0; i < 4; ++i)
                            {
                                if (i > 0)
                                    hexNum <<= 4;
                                Color color = bitmap.GetPixel(tileXOffset + x + 3 - i, tileYOffset + y);
                                int index = ColorToPaletteIndex(palette, color);
                                hexNum += (UInt16)index;
                            }
                            sbOutput.AppendFormat("0x{0:X4}, ", hexNum);
                            if ((hexCount + 1) % c_arrayNewlineCount == 0)
                            {
                                sbOutput.Append("\n\t" + tabs);
                            }
                            ++hexCount;
                        }
                    }
                }
            }

            sbOutput.Append("\n\n");

            return hexCount;
        }
    }
}
