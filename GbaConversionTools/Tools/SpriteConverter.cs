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
        public struct UVs
        {
            public int x, y, width, height;
        }

        const int c_arrayNewlineCount = 5;

        const string compressionComment = "// Bit0 - 3   Data size in bit units(normally 4 or 8). May be reserved/unused for other compression types \n" + namespaceTabs +
         "// Bit4-7   Compressed type \n" + namespaceTabs +
         "// Bit8-31  Unused, generated in-game as 24bit size of decompressed data in bytes, probably \n";

        const string TAB_CHAR = Defines.TAB_CHAR;
        const string namespaceTabs = TAB_CHAR;
        const string STR_U8 = Defines.STR_U8;
        const string STR_U16 = Defines.STR_U16;
        const string STR_U32 = Defines.STR_U32;
        const string VARPREFIXES = "extern " + Defines.STR_EWRAM_DATA + " const ";
        const string NAMESPACE_FORMAT                   = "namespace " + Defines.STR_SPRITEMAP_NAMESPC_PREFIX + "{0} \n{{\n";
        const string VAR_HEADER_PALLETLENGTH_FORMAT     = namespaceTabs + VARPREFIXES + STR_U8 + " paletteLength = {0};\n";
        const string VAR_HEADER_DATALENGTH_FORMAT       = namespaceTabs + VARPREFIXES + STR_U32 + " dataLength = {0};\n";
        const string VAR_PALLET                         = namespaceTabs + VARPREFIXES + STR_U16 + " palette[] = \n";
        const string VAR_SPRITECOUNT_FORMAT             = namespaceTabs + VARPREFIXES + STR_U32 + " spriteCount = {0};\n";
        const string VAR_WIDTHMAP                       = namespaceTabs + VARPREFIXES + STR_U8 + " widthMap[] = \n";
        const string VAR_HEIGHTMAP                      = namespaceTabs + VARPREFIXES + STR_U8 + " heightMap[] = \n";
        const string VAR_OFFSETS                        = namespaceTabs + VARPREFIXES + STR_U32 + " offsets[] = \n";
        const string VAR_HEADER_COMPRESSION_FORMAT      = namespaceTabs + compressionComment + namespaceTabs + VARPREFIXES + STR_U32 + " compressionTypeSize = {0}; \n";
        const string VAR_DATA                           = namespaceTabs + VARPREFIXES + STR_U32 + " data[] = \n";

        public void Convert(string inputPath, string outputPath, Bitmap bitmap, UVs[] sliceCoordinates)
        {
            string namespaceName = string.Format(NAMESPACE_FORMAT, Path.GetFileName(Path.GetFileNameWithoutExtension(inputPath)));

            StringBuilder sb = new StringBuilder();
            Size size = bitmap.Size;
            if (size.Width % TileConfig.c_TILEWIDTH != 0 || size.Height % TileConfig.c_TILEHEIGHT != 0)
            {
                throw new Exception("Size not compatible with GBA tiles. Width and height of pixels must be multiples of 8.");
            }

            Console.WriteLine("Processing colour palette");

            int xStart = 0;
            int yStart = 0;
            int width = bitmap.Width;
            int height = bitmap.Height;
            Color[] preProcessedPalette = PaletteHelper.GeneratePaletteFromImage(bitmap, xStart, yStart, width, height);

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

            Compression.CompressionType compressionType = Compression.CompressionType.BitPacked;
            uint maxColours = MathX.IsPowerOf2((uint)preProcessedPalette.Length) ? (uint)preProcessedPalette.Length : MathX.NextPowerOf2((uint)preProcessedPalette.Length);
            uint destBpp = (uint)MathX.IndexOfHighestSetBit(maxColours);

            if (!MathX.IsPowerOf2(destBpp))
            {
                destBpp = MathX.NextPowerOf2(destBpp);
            }

            Console.WriteLine(string.Format("Compression type = {0}", compressionType.ToString()));
            Console.WriteLine(string.Format("Destination Bit Depth = {0}", destBpp));

            List<int> dataOffsets = new List<int>();
            List<StringBuilder> spriteData = new List<StringBuilder>();
            dataOffsets.Add(0);
            int totalData = 0;

            // Collect data and add offsets
            {
                Console.WriteLine("Processing sprite data");

                for (int i = 0; i < sliceCoordinates.Length; ++i)
                {
                    StringBuilder dataSb = new StringBuilder();
                    UVs slice = sliceCoordinates[i];
                    int spriteWidth = slice.width, spriteHeight = slice.height;
                    int dataCount = WriteSpriteData(dataSb, bitmap, preProcessedPalette, spriteWidth, spriteHeight, slice.x, slice.y, compressionType, destBpp);

                    spriteData.Add(dataSb);

                    // Add offsets
                    if (i < sliceCoordinates.Length - 1)
                        dataOffsets.Add(dataOffsets[i] + dataCount);

                    totalData += dataCount;
                }
            }

            sb.Append(namespaceName);

            WriteHeader(sliceCoordinates, preProcessedPalette, totalData, compressionType, destBpp, sb);
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

            // Write offsets
            {
                const string tabs = namespaceTabs + TAB_CHAR;

                sb.Append(VAR_OFFSETS);
                sb.Append(namespaceTabs + "{\n" + tabs);
                for (int i = 0; i < dataOffsets.Count; i++)
                {
                    sb.AppendFormat("{0}, ", dataOffsets[i]);

                    if ((i + 1) % c_arrayNewlineCount == 0)
                        sb.AppendFormat("\n" + tabs);
                }

                sb.Append('\n' + namespaceTabs + "};\n\n");
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

        void WriteHeader(UVs[] sliceCoordinates, Color[] palette, int size, Compression.CompressionType compressionType, uint bpp, StringBuilder sb)
        {
            UInt32 compressionTypeSize = Compression.ToGBACompressionHeader(compressionType, bpp);

            sb.Append(namespaceTabs + "// File Header\n");
            sb.AppendFormat(VAR_SPRITECOUNT_FORMAT, sliceCoordinates.Length);
            sb.AppendFormat(VAR_HEADER_PALLETLENGTH_FORMAT, palette.Length);
            sb.AppendFormat(VAR_HEADER_DATALENGTH_FORMAT, size);

            sb.Append('\n');
            sb.AppendFormat(VAR_HEADER_COMPRESSION_FORMAT, compressionTypeSize);

            sb.Append('\n');
        }

        void WritePalette(Color[] palette, StringBuilder sb)
        {
            sb.Append(VAR_PALLET);
            sb.Append(namespaceTabs + "{\n\t" + namespaceTabs);
            for (int i = 0; i < palette.Length; ++i)
            {
                Color color = palette[i];
                UInt16 rbgColor = (UInt16)(PaletteHelper.ScaleToRgb16(color.R) + (PaletteHelper.ScaleToRgb16(color.G) << 5) + (PaletteHelper.ScaleToRgb16(color.B) << 10));

                sb.AppendFormat("0x{0:X4}, ", rbgColor);
                if ((i + 1) % c_arrayNewlineCount == 0)
                {
                    sb.Append("\n\t" + namespaceTabs);
                }
            }
            sb.Append("\n" + namespaceTabs + "};\n\n");
        }

        void WriteSprite(
            StringBuilder sbOutput, 
            Bitmap bitmap, 
            Color[] palette, 
            int width, 
            int height, 
            int startOffsetX, 
            int startOffsetY, 
            uint destBpp, 
            Compression.CompressionType compressionType)
        {
            const string tabs = namespaceTabs + TAB_CHAR;

            sbOutput.Append(tabs + "{\n\t" + tabs);
            WriteSpriteData(sbOutput, bitmap, palette, width, height, startOffsetX, startOffsetY, compressionType, destBpp);
            sbOutput.Append("\n" + tabs + "},\n");
        }

        int WriteSpriteData(
            StringBuilder sbOutput, 
            Bitmap bitmap, 
            Color[] palette, 
            int width, 
            int height, 
            int xPos, 
            int yPos, 
            Compression.CompressionType compressionType, 
            uint destBpp)
        {
            const string tabs = namespaceTabs;

            int tilesWide = width / TileConfig.c_TILEWIDTH;
            int tilesTall = height / TileConfig.c_TILEHEIGHT;
            int totalTiles = (width * height) / (TileConfig.c_TILEWIDTH * TileConfig.c_TILEHEIGHT);
            int hexCount = 0;
            List<int> colourPaletteIndicies = new List<int>();
            sbOutput.Append(namespaceTabs + TAB_CHAR);

            for (int tileY = 0; tileY < tilesTall; ++tileY)
            {
                for (int tileX = 0; tileX < tilesWide; ++tileX)
                {
                    int tileXOffset = xPos + tileX * TileConfig.c_TILEWIDTH;
                    int tileYOffset = yPos + tileY * TileConfig.c_TILEHEIGHT;

                    for (int y = 0; y < TileConfig.c_TILEHEIGHT; ++y)
                    {
                        for (int x = 0; x < TileConfig.c_TILEWIDTH; ++x)
                        {
                            Color color = bitmap.GetPixel(tileXOffset + x, tileYOffset + y);
                            int index = PaletteHelper.ColorToPaletteIndex(palette, color);
                            colourPaletteIndicies.Add(index);
                        }
                    }
                }
            }

            List<UInt32> compressedIndicies;

            switch (compressionType)
            {
                case Compression.CompressionType.BitPacked:
                    {
                        Compression.BitPack(colourPaletteIndicies, destBpp, out compressedIndicies);
                    }
                    break;
                default:
                    throw new NotImplementedException();
            }

            foreach (var hexNum in compressedIndicies)
            {
                sbOutput.AppendFormat("0x{0:X8}, ", hexNum);
                if ((hexCount + 1) % c_arrayNewlineCount == 0)
                {
                    sbOutput.Append("\n\t" + tabs);
                }
                ++hexCount;
            }

            sbOutput.Append("\n\n");

            return hexCount;
        }
    }
}
