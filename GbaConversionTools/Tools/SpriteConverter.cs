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

        public void Convert(string inputPath, string outputPath, Bitmap bitmap, UVs[] sliceCoordinates)
        {
            CppWriter cppWriter = new CppWriter(Path.GetFileName(Path.GetFileNameWithoutExtension(inputPath)), outputPath);

            Size size = bitmap.Size;
            if (size.Width % TileConfig.TileWidth != 0 || size.Height % TileConfig.TileHeight != 0)
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

            Console.WriteLine(String.Format("Palette length: {0}", preProcessedPalette.Length));

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
            List<List<UInt32>> spriteData = new List<List<UInt32>>();
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
                    List<UInt32> data = WriteSpriteData(bitmap, preProcessedPalette, spriteWidth, spriteHeight, slice.x, slice.y, compressionType, destBpp);

                    spriteData.Add(data);

                    // Add offsets
                    if (i < sliceCoordinates.Length - 1)
                        dataOffsets.Add(dataOffsets[i] + data.Count);

                    totalData += data.Count;
                }
            }

            WriteHeader(cppWriter, sliceCoordinates, preProcessedPalette, totalData, compressionType, destBpp);
            WritePalette(cppWriter, preProcessedPalette);

            // Write width
            {
                for (int i = 0; i < sliceCoordinates.Length; ++i)
                {
                    int spriteWidth = sliceCoordinates[i].width;
                    cppWriter.Write((byte)spriteWidth);
                }
            }

            // Write height
            {
                for (int i = 0; i < sliceCoordinates.Length; ++i)
                {
                    int spriteHeight = sliceCoordinates[i].height;
                    cppWriter.Write((byte)spriteHeight);
                }
            }

            // Write offsets
            {
                for (int i = 0; i < dataOffsets.Count; i++)
                {
                    cppWriter.Write((UInt32)dataOffsets[i]);
                }
            }

            // Write data
            {
                foreach(var dataList in spriteData)
                {
                    foreach (UInt32 data in dataList)
                    {
                        cppWriter.Write((UInt32)data);
                    }
                }
            }

            Console.WriteLine("Sprite \"" + outputPath + "\" successfully converted");

            cppWriter.Finalise();
        }

        void WriteHeader(CppWriter writer, UVs[] sliceCoordinates, Color[] palette, int dataLength, Compression.CompressionType compressionType, uint bpp)
        {
            UInt32 compressionTypeSize = Compression.ToGBACompressionHeader(compressionType, bpp);

            writer.Write((UInt32)sliceCoordinates.Length);
            writer.Write((byte)palette.Length);
            writer.Write((UInt32)dataLength);
            writer.Write((UInt32)compressionTypeSize);
        }

        void WritePalette(CppWriter writer, Color[] palette)
        {
            for (int i = 0; i < palette.Length; ++i)
            {
                Color colour = palette[i];
                UInt16 rbgColor = PaletteHelper.ToRgb16(colour);

                writer.Write(rbgColor);
            }
        }

        List<UInt32> WriteSpriteData(
            Bitmap bitmap, 
            Color[] palette, 
            int width, 
            int height, 
            int xPos, 
            int yPos, 
            Compression.CompressionType compressionType, 
            uint destBpp)
        {
            int tilesWide = width / TileConfig.TileWidth;
            int tilesTall = height / TileConfig.TileHeight;
            int totalTiles = (width * height) / (TileConfig.TileWidth * TileConfig.TileHeight);
            List<int> colourPaletteIndicies = new List<int>();

            for (int tileY = 0; tileY < tilesTall; ++tileY)
            {
                for (int tileX = 0; tileX < tilesWide; ++tileX)
                {
                    int tileXOffset = xPos + tileX * TileConfig.TileWidth;
                    int tileYOffset = yPos + tileY * TileConfig.TileHeight;

                    for (int y = 0; y < TileConfig.TileHeight; ++y)
                    {
                        for (int x = 0; x < TileConfig.TileWidth; ++x)
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

            return compressedIndicies;
        }
    }
}
