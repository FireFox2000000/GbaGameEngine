using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;

namespace GbaConversionTools.Tools
{
    internal class UiAtlusConverter
    {
        public struct UiAtlusBitmap
        {
            public string name;
            public Bitmap bitmap;
        }

        public void Convert(string inputPath, string outputPath, IList<UiAtlusBitmap> atlusBitmaps, byte paletteBankIndexOffset = 0)
        {
            VerifyUiEnumNames(atlusBitmaps);

            CppWriter cppWriter = new CppWriter(Path.GetFileName(Path.GetFileNameWithoutExtension(inputPath)), outputPath);

            Console.WriteLine("Processing master palette");

            Color[] masterPalette;
            TilemapConverter.Tile[] masterTileSet;
            TilemapConverter.TileMap[] tileMapList;

            List<Bitmap> bitmaps = new List<Bitmap>();
            foreach (var bitmap in atlusBitmaps)
            {
                bitmaps.Add(bitmap.bitmap);
            }

            TilemapConverter.GenerateTileMaps(bitmaps, paletteBankIndexOffset, out masterPalette, out masterTileSet, out tileMapList);
            TilemapConverter.GBAMapData gbaMapData = TilemapConverter.GenerateMapData(tileMapList, paletteBankIndexOffset);

            Console.WriteLine("Total unique tiles = " + masterTileSet.Length);
            Console.WriteLine("Processing tilemaps");

            Compression.CompressionType compressionType = Compression.CompressionType.BitPacked;
            uint destBpp = TilemapConverter.CalculateDestBitsPerPixel(masterTileSet);

            List<UInt32> tileSetData = TilemapConverter.GenerateTileSetData(masterTileSet, compressionType, destBpp);

            // Write palette
            {
                cppWriter.Write(paletteBankIndexOffset);
                cppWriter.Write((byte)masterPalette.Length);

                for (int i = 0; i < masterPalette.Length; ++i)
                {
                    Color colour = masterPalette[i];
                    UInt16 rbgColor = PaletteHelper.ToRgb16(colour);
                    cppWriter.Write(rbgColor);
                }
            }

            // Write tileset
            {
                // Compression flags
                UInt32 compressionTypeSize = Compression.ToGBACompressionHeader(compressionType, destBpp);
                cppWriter.Write(compressionTypeSize);

                // Actual data
                cppWriter.Write((UInt32)tileSetData.Count);
                for (int i = 0; i < tileSetData.Count; ++i)
                {
                    cppWriter.Write(tileSetData[i]);
                }
            }

            // Write tile maps
            {
                cppWriter.Write((byte)tileMapList.Length);
                cppWriter.Write((UInt32)gbaMapData.screenEntries.Length);

                // Don't write this out. We have a crap-ton of maps in the ui atlus and we're not going to render it as static or dynamic anyway
                //cppWriter.Write(gbaMapData.mapIsDynamicBitMask);

                // Width and height arrays
                foreach (int width in gbaMapData.widthLists)
                {
                    cppWriter.Write((byte)width);
                }

                foreach (int height in gbaMapData.heightLists)
                {
                    cppWriter.Write((byte)height);
                }

                foreach (var mapEntry in gbaMapData.screenEntries)
                {
                    cppWriter.Write(mapEntry.m_data);
                }
            }

            cppWriter.Finalise();

            // Append UI enum list to the header file
            {
                string headerFilePath = cppWriter.HeaderFilePath;
                using (var writer = File.AppendText(headerFilePath))
                {
                    writer.WriteLine("");

                    int indentationLevel = 1;
                    writer.WriteLine(CppWriter.PrefixIndentation("enum Enum", indentationLevel));
                    writer.WriteLine(CppWriter.PrefixIndentation("{", indentationLevel));

                    ++indentationLevel;
                    {
                        // Append the UI enums to the header file
                        foreach (var atlusBitmap in atlusBitmaps)
                        {
                            string enumName = atlusBitmap.name;

                            writer.WriteLine(CppWriter.PrefixIndentation(String.Format("{0},", enumName), indentationLevel));
                        }
                    }
                    --indentationLevel;

                    writer.WriteLine(CppWriter.PrefixIndentation("};", indentationLevel));
                }
            }

            Console.WriteLine("Ui Atlus \"" + outputPath + "\" successfully converted");
        }

        bool VerifyUiEnumNames(IList<UiAtlusBitmap> atlusBitmaps)
        {
            for (int i = 0; i < atlusBitmaps.Count; ++i)
            {
                // Check for duplicate names
                for (int j = i + 1; j < atlusBitmaps.Count; ++j)
                {
                    if (atlusBitmaps[i].name == atlusBitmaps[j].name)
                    {
                        throw new Exception(String.Format("Found duplicate name at indexes {0} and {1}. Name was {2}", i, j, atlusBitmaps[i].name));
                    }
                }

                // Check for invalid characters
                {
                    string enumName = atlusBitmaps[i].name;
                    //if (enumName.Contains)
                }
            }

            return true;
        }
    }
}
