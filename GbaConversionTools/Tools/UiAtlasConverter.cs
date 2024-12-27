using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Numerics;

namespace GbaConversionTools.Tools
{
    internal class UiAtlasConverter
    {
        public struct UiAtlasBitmap
        {
            public string name;
            public Bitmap bitmap;
        }

        public struct UiAtlasUvs
        {
            public string name;
            public Tools.SpriteConverter.UVs uvs;
        }

        public struct UiAtlasConfig
        {
            public struct FontProperties
            {
                // Uv index of the first ascii character
                public int uvAsciiStart;

                // The start of the supported ascii range. Generally ' ' or '!'
                public char firstAsciiCharacter;

                // Max size in tiles of the font, for line break etc. 
                public Vector2 fixedCharacterSize;
            }

            public FontProperties fontProperties;
            public UiAtlasUvs[] uvs;
        }

        public void Convert(string inputPath, string outputPath, in UiAtlasConfig uiAtlasConfig, IList<UiAtlasBitmap> atlasBitmaps, byte paletteBankIndexOffset = 0)
        {
            VerifyUiEnumNames(atlasBitmaps);

            CppWriter cppWriter = new CppWriter(Path.GetFileName(Path.GetFileNameWithoutExtension(inputPath)), outputPath);

            Console.WriteLine("Processing master palette");

            Color[] masterPalette;
            TilemapConverter.Tile[] masterTileSet;
            TilemapConverter.TileMap[] tileMapList;

            List<Bitmap> bitmaps = new List<Bitmap>();
            foreach (var bitmap in atlasBitmaps)
            {
                bitmaps.Add(bitmap.bitmap);
            }

            TilemapConverter.GenerateTileMaps(bitmaps, paletteBankIndexOffset, out masterPalette, out masterTileSet, out tileMapList, true);
            TilemapConverter.GBAMapData gbaMapData = TilemapConverter.GenerateMapData(tileMapList, paletteBankIndexOffset);

            // Add clear tile at the end if not already present
            int clearTileIndex = -1;
            {
                TilemapConverter.Tile clearTile = new TilemapConverter.Tile();
                List<TilemapConverter.Tile> tiles = new List<TilemapConverter.Tile>();
                tiles.AddRange(masterTileSet);

                // Search
                for (int i = 0; i < tiles.Count; ++i)
                {
                    if (tiles[i].Compare(clearTile) == TilemapConverter.Tile.ComparisonResult.Same)
                    {
                        clearTileIndex = i;
                        break;
                    }
                }

                // Not found, append
                if (clearTileIndex == -1)
                {
                    clearTileIndex = tiles.Count;
                    tiles.Add(clearTile);
                }

                masterTileSet = tiles.ToArray();
            }

            TilemapConverter.GBAScreenEntry clearScreenEntry = new TilemapConverter.GBAScreenEntry();
            clearScreenEntry.SetTileIndex(clearTileIndex);
            clearScreenEntry.SetPalIndex(paletteBankIndexOffset);

            Console.WriteLine("Total unique tiles = " + masterTileSet.Length);
            Console.WriteLine("Processing tilemaps");

            Compression.CompressionType compressionType = Compression.CompressionType.BitPacked;
            uint destBpp = TilemapConverter.CalculateDestBitsPerPixel(masterTileSet);

            List<UInt32> tileSetData = TilemapConverter.GenerateTileSetData(masterTileSet, compressionType, destBpp);

            // Write font properties
            {
                cppWriter.Write(uiAtlasConfig.fontProperties.uvAsciiStart);
                cppWriter.Write(uiAtlasConfig.fontProperties.firstAsciiCharacter);
                cppWriter.Write((int)(uiAtlasConfig.fontProperties.fixedCharacterSize.X));
                cppWriter.Write((int)(uiAtlasConfig.fontProperties.fixedCharacterSize.Y));
            }

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
                
                // Clear screen entry
                cppWriter.Write(clearScreenEntry.m_data);

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

                // Don't write this out. We have a crap-ton of maps in the ui atlas and we're not going to render it as static or dynamic anyway
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
                    writer.WriteLine(CppWriter.PrefixIndentation("enum UiAtlasObject", indentationLevel));
                    writer.WriteLine(CppWriter.PrefixIndentation("{", indentationLevel));

                    ++indentationLevel;
                    {
                        // Append the UI enums to the header file
                        foreach (var atlasBitmap in atlasBitmaps)
                        {
                            string enumName = atlasBitmap.name;

                            writer.WriteLine(CppWriter.PrefixIndentation(String.Format("{0},", enumName), indentationLevel));
                        }
                    }
                    --indentationLevel;

                    writer.WriteLine(CppWriter.PrefixIndentation("};", indentationLevel));
                }
            }

            Console.WriteLine("Ui Atlas \"" + outputPath + "\" successfully converted");
        }

        bool VerifyUiEnumNames(IList<UiAtlasBitmap> atlasBitmaps)
        {
            for (int i = 0; i < atlasBitmaps.Count; ++i)
            {
                // Check for duplicate names
                for (int j = i + 1; j < atlasBitmaps.Count; ++j)
                {
                    if (atlasBitmaps[i].name == atlasBitmaps[j].name)
                    {
                        throw new Exception(String.Format("Found duplicate name at indexes {0} and {1}. Name was {2}", i, j, atlasBitmaps[i].name));
                    }
                }

                // Check for invalid characters
                {
                    string enumName = atlasBitmaps[i].name;
                    //if (enumName.Contains)
                }
            }

            return true;
        }
    }
}
