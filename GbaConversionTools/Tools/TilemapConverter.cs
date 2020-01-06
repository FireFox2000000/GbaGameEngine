using System;
using System.Text;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Collections.Generic;

namespace GbaConversionTools.Tools
{
    class TilemapConverter
    {
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

        const string NAMESPACE_FORMAT = "namespace " + Defines.STR_TILEMAP_NAMESPC_PREFIX + "{0} \n{{\n";
        const string VAR_HEADER_PALLETLENGTH_FORMAT = namespaceTabs + VARPREFIXES + STR_U8 + " paletteLength = {0};\n";
        const string VAR_HEADER_TILESETLENGTH_FORMAT = namespaceTabs + VARPREFIXES + STR_U32 + " tilesetLength = {0};\n";
        const string VAR_HEADER_TILEMAPLENGTH_FORMAT = namespaceTabs + VARPREFIXES + STR_U16 + " mapLength = {0};\n";

        const string VAR_PALLET = namespaceTabs + VARPREFIXES + STR_U16 + " palette[] = \n";
        const string VAR_TILESET_COMPRESSION_FORMAT = namespaceTabs + compressionComment + namespaceTabs + VARPREFIXES + STR_U32 + " tileSetCompressionTypeSize = {0}; \n";
        const string VAR_TILESET = namespaceTabs + VARPREFIXES + STR_U32 + " tileset[] = \n";
        const string VAR_TILEMAP = namespaceTabs + VARPREFIXES + STR_U16 + " map[] = \n";

        const int MAX_UNIQUE_TILES = 1024;
        const int GBA_MAP_TILE_WIDTH = 32;
        const int GBA_MAP_TILE_HEIGHT = 32;

        class ProcessedBitmapContainer
        {
            public int paletteIndex = -1;
            public Bitmap bitmap;
            public int bitmapIndex = -1;
            public Tile[,] rawTileMap;
        }

        class ProcessedPaletteContainer
        {
            public Color[] palette;
            public int bitmapIndex = -1;
        }

        class ProcessedPaletteComparer : IComparer<ProcessedPaletteContainer>
        {
            public int Compare(ProcessedPaletteContainer x, ProcessedPaletteContainer y)
            {
                if (x.palette.Length == y.palette.Length || (x.palette.Length <= PaletteHelper.PALETTE_LENGTH_4BBP && y.palette.Length <= PaletteHelper.PALETTE_LENGTH_4BBP))
                {
                    return 0;
                }

                if (x.palette.Length <= PaletteHelper.PALETTE_LENGTH_4BBP)
                    return -1;

                if (y.palette.Length <= PaletteHelper.PALETTE_LENGTH_4BBP)
                    return 1;

                return y.palette.Length - x.palette.Length;
            }
        }

        public void Convert(string inputPath, string outputPath, Bitmap inputBitmap)
        {
            string namespaceName = string.Format(NAMESPACE_FORMAT, Path.GetFileName(Path.GetFileNameWithoutExtension(inputPath)));

            List<Bitmap> bitmaps = new List<Bitmap>();
            bitmaps.Add(inputBitmap);

            List<ProcessedPaletteContainer> bitmapPalettes = new List<ProcessedPaletteContainer>();

            Console.WriteLine("Processing master palette");

            // Validate bitmaps and collect palettes
            for (int bitmapIndex = 0; bitmapIndex < bitmaps.Count; ++bitmapIndex)
            {
                Bitmap bitmap = bitmaps[bitmapIndex];

                Size size = bitmap.Size;
                if (size.Width % TileConfig.c_TILEWIDTH != 0 || size.Height % TileConfig.c_TILEHEIGHT != 0)
                {
                    throw new Exception("Size not compatible with GBA tiles. Width and height of pixels must be multiples of 8.");
                }

                int xStart = 0;
                int yStart = 0;
                int width = bitmap.Width;
                int height = bitmap.Height;
                Color[] preProcessedPalette = PaletteHelper.GeneratePaletteFromImage(bitmap, xStart, yStart, width, height);

                // Validate pallet length
                {
                    if (preProcessedPalette.Length > PaletteHelper.MAX_PALETTE_LENGTH)
                    {
                        throw new Exception(string.Format("Palette length ({0}) out of range for the GBA ({1})", preProcessedPalette.Length, PaletteHelper.MAX_PALETTE_LENGTH));
                    }
                }

                ProcessedPaletteContainer palContainer = new ProcessedPaletteContainer();
                palContainer.bitmapIndex = bitmapIndex;
                palContainer.palette = preProcessedPalette;

                bitmapPalettes.Add(palContainer);
            }

            // Sort bitmap palette. Anything more than 16 colours should be grouped next to each other. Less than 16 should always be a full 16.
            bitmapPalettes.Sort(new ProcessedPaletteComparer());

            // Get palettes of each image, merge into a master palette
            List<Color> masterPaletteList = new List<Color>();
            List<ProcessedBitmapContainer> processedBitmapList = new List<ProcessedBitmapContainer>();
            {
                Color transparencyColour = Color.FromArgb(0);

                for (int i = 0; i < bitmapPalettes.Count; ++i)
                {
                    var bp = bitmapPalettes[i];

                    bool pal4bbp = bp.palette.Length <= PaletteHelper.PALETTE_LENGTH_4BBP;
                    if (pal4bbp)
                    {
                        // Make sure our 4bbp palette align properly
                        while (masterPaletteList.Count % PaletteHelper.PALETTE_LENGTH_4BBP != 0)
                        {
                            masterPaletteList.Add(transparencyColour);
                        }
                    }

                    int currentPaletteIndex = masterPaletteList.Count / PaletteHelper.PALETTE_LENGTH_4BBP;
                    masterPaletteList.AddRange(bp.palette);

                    ProcessedBitmapContainer pbc = new ProcessedBitmapContainer();
                    pbc.bitmapIndex = bp.bitmapIndex;
                    pbc.paletteIndex = pal4bbp ? currentPaletteIndex : -1;
                    pbc.bitmap = bitmaps[bp.bitmapIndex];

                    processedBitmapList.Add(pbc);
                }
            }

            Color[] masterPalette = masterPaletteList.ToArray();

            Console.WriteLine("Total colours found = " + masterPalette.Length);

            Console.WriteLine("Processing master tileset");

            // Get all unique tiles sorted via palette indicies. Need to check if local palette was greater than 16 or not
            List<Tile> uniqueTileSetList = new List<Tile>();
            foreach(ProcessedBitmapContainer pbc in processedBitmapList)
            {
                Tile[,] rawTileMap = BitmapToTileArray(pbc.bitmap, masterPalette, pbc.paletteIndex);
                pbc.rawTileMap = rawTileMap;

                FillUniqueTileset(rawTileMap, uniqueTileSetList);
            }

            Tile[] masterTileSet = uniqueTileSetList.ToArray();

            if (masterTileSet.Length > MAX_UNIQUE_TILES)
                throw new Exception(string.Format("Too many tiles present in tilemap. Max tiles = {0}. Total titles found = {1}", MAX_UNIQUE_TILES, masterTileSet.Length));

            Console.WriteLine("Total unique tiles = " + masterTileSet.Length);
            Console.WriteLine("Processing tilemaps");

            // Generate tilemaps from master list. Remember palette index, but we should be able to forget about it. We can overlap tiles that are the same but different colours.
            List<TileMap> tileMapList = new List<TileMap>();
            foreach(ProcessedBitmapContainer pbc in processedBitmapList)
            {
                TileMap tilemap = new TileMap(pbc.rawTileMap, masterTileSet, pbc.paletteIndex);
                tileMapList.Add(tilemap);
            }

            Compression.CompressionType compressionType = Compression.CompressionType.BitPacked;
            uint destBpp = 4;

            foreach (Tile tile in masterTileSet)
            {
                foreach (int index in tile.paletteIndicies)
                {
                    if (index >= PaletteHelper.PALETTE_LENGTH_4BBP)
                    {
                        destBpp = 8;
                        goto WriteMasterTileSetToSb;
                    }
                }
            }

        WriteMasterTileSetToSb:

            int tilesetLength;
            StringBuilder tilesetSb = new StringBuilder();
            WriteTileSet(masterTileSet, tilesetSb, compressionType, destBpp, out tilesetLength);

            StringBuilder sb = new StringBuilder();

            sb.Append(namespaceName);

            WriteHeader(masterPalette, tilesetLength, sb);
            WritePalette(masterPalette, sb);

            // Real write tileset
            sb.Append(tilesetSb);

            // Write tile maps
            WriteMapData(tileMapList, sb);

            sb.Append("}\n");

            Console.WriteLine("Tilemap \"" + outputPath + "\" successfully converted");
            File.WriteAllText(outputPath, sb.ToString());
        }

        void WriteHeader(Color[] palette, int tilesetLength, StringBuilder sb)
        {
            sb.Append(namespaceTabs + "// File Header\n");
            sb.AppendFormat(VAR_HEADER_PALLETLENGTH_FORMAT, palette.Length);
            sb.AppendFormat(VAR_HEADER_TILESETLENGTH_FORMAT, tilesetLength);

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

        void WriteTileSet(Tile[] tiles, StringBuilder sb, Compression.CompressionType compressionType, uint destBpp, out int totalLength)
        {
            UInt32 compressionTypeSize = Compression.ToGBACompressionHeader(compressionType, destBpp);

            sb.Append('\n');
            sb.AppendFormat(VAR_TILESET_COMPRESSION_FORMAT, compressionTypeSize);

            sb.Append(VAR_TILESET);
            sb.Append(namespaceTabs + "{\n\t" + namespaceTabs);

            totalLength = 0;

            foreach (Tile tile in tiles)
            {
                List<UInt32> compressedIndicies;
                List<int> linearIndicies = new List<int>();

                for (int j = 0; j < tile.paletteIndicies.GetLength(1); ++j)
                {
                    for (int i = 0; i < tile.paletteIndicies.GetLength(0); ++i)
                    {
                        linearIndicies.Add(tile.paletteIndicies[i, j]);
                    }
                }

                Compression.BitPack(linearIndicies, destBpp, out compressedIndicies);

                int hexCount = 0;
                const string tabs = namespaceTabs;
                foreach (var hexNum in compressedIndicies)
                {
                    sb.AppendFormat("0x{0:X8}, ", hexNum);
                    if ((hexCount + 1) % c_arrayNewlineCount == 0)
                    {
                        sb.Append("\n\t" + tabs);
                    }
                    ++totalLength;
                    ++hexCount;
                }

                sb.Append("\n\n\t" + tabs);
            } 

            sb.Append("\n" + namespaceTabs + "};\n\n");
        }

        void WriteMapData(List<TileMap> tilemaps, StringBuilder sb)
        {
            const string tabs = namespaceTabs;

            List<GBAScreenEntry> seList = new List<GBAScreenEntry>();
            List<int> mapStartOffsets = new List<int>();

            foreach (var tilemap in tilemaps)
            {
                mapStartOffsets.Add(seList.Count);

                // GBA nesting: https://www.coranac.com/tonc/text/regbg.htm 9.3.1
                for (int mapStartY = 0; mapStartY < tilemap.mapData.GetLength(1); mapStartY += GBA_MAP_TILE_HEIGHT)
                {
                    for (int mapStartX = 0; mapStartX < tilemap.mapData.GetLength(0); mapStartX += GBA_MAP_TILE_WIDTH)
                    {
                        for (int j = mapStartY; j < mapStartY + GBA_MAP_TILE_HEIGHT; ++j)
                        {
                            for (int i = mapStartX; i < mapStartX + GBA_MAP_TILE_WIDTH; ++i)
                            {
                                var currentMapData = tilemap.mapData[i, j];

                                GBAScreenEntry screenEntry = new GBAScreenEntry();
                                screenEntry.SetTileIndex(currentMapData.tilesetIndex);

                                if ((currentMapData.flags & TileMap.FlippingFlags.Horizontal) != 0)
                                    screenEntry.SetHFlipFlag();

                                if ((currentMapData.flags & TileMap.FlippingFlags.Vertical) != 0)
                                    screenEntry.SetVFlipFlag();

                                if (tilemap.paletteIndex >= 0)
                                    screenEntry.SetPalIndex(tilemap.paletteIndex);

                                seList.Add(screenEntry);
                            }
                        }
                    }
                } 
            }

            GBAScreenEntry[] mapEntries = seList.ToArray();
            sb.AppendFormat(VAR_HEADER_TILEMAPLENGTH_FORMAT, mapEntries.Length);

            sb.Append(VAR_TILEMAP);
            sb.Append(namespaceTabs + "{\n\t" + namespaceTabs);

            int hexCount = 0;

            foreach (var mapEntry in mapEntries)
            {
                sb.AppendFormat("0x{0:X4}, ", mapEntry.m_data);
                if ((hexCount + 1) % c_arrayNewlineCount == 0)
                {
                    sb.Append("\n\t" + tabs);
                }

                ++hexCount;
            }

            sb.Append("\n" + namespaceTabs + "};\n\n");
        }

        class GBAScreenEntry
        {
            public UInt16 m_data;

            public void SetTileIndex(int index)
            {
                m_data |= (UInt16)(index & 0b1111111111);
            }

            public void SetHFlipFlag()
            {
                m_data |= 1 << 10;
            }

            public void SetVFlipFlag()
            {
                m_data |= 1 << 11;
            }

            public void SetPalIndex(int index)
            {
                m_data |= (UInt16)((index & 0b1111) << 0xc);
            }
        }

        class Tile
        {
            public enum ComparisonResult
            {
                Different,
                Same,
                FlipH,
                FlipV,
                FlipHFlipV,
            }

            public int[,] paletteIndicies = new int[TileConfig.c_TILEWIDTH, TileConfig.c_TILEHEIGHT];

            public bool Equals(Tile that)
            {
                for (int i = 0; i < paletteIndicies.GetLength(0); ++i)
                {
                    for (int j = 0; j < paletteIndicies.GetLength(1); ++j)
                    {
                        if (paletteIndicies[i, j] != that.paletteIndicies[i, j])
                        {
                            return false;
                        }
                    }
                }

                return true;
            }

            public ComparisonResult Compare(Tile that)
            {
                if (Equals(that))
                {
                    return ComparisonResult.Same;
                }

                Tile flipH = this.FlipHorizontal();
                if (flipH.Equals(that))
                {
                    return ComparisonResult.FlipH;
                }

                if (this.FlipVertical().Equals(that))
                {
                    return ComparisonResult.FlipV;
                }

                if (flipH.FlipVertical().Equals(that))
                {
                    return ComparisonResult.FlipHFlipV;
                }

                return ComparisonResult.Different;
            }

            Tile FlipHorizontal()
            {
                int[,] flippedPaletteIndicies = new int[TileConfig.c_TILEWIDTH, TileConfig.c_TILEHEIGHT];

                for (int i = 0; i < paletteIndicies.GetLength(0); ++i)
                {
                    for (int j = 0; j < paletteIndicies.GetLength(1); ++j)
                    {
                        flippedPaletteIndicies[paletteIndicies.GetLength(0) - i - 1, j] = paletteIndicies[i, j];
                    }
                }

                Tile tile = new Tile();
                tile.paletteIndicies = flippedPaletteIndicies;
                return tile;
            }

            Tile FlipVertical()
            {
                int[,] flippedPaletteIndicies = new int[TileConfig.c_TILEWIDTH, TileConfig.c_TILEHEIGHT];

                for (int i = 0; i < paletteIndicies.GetLength(0); ++i)
                {
                    for (int j = 0; j < paletteIndicies.GetLength(1); ++j)
                    {
                        flippedPaletteIndicies[i, paletteIndicies.GetLength(1) - j - 1] = paletteIndicies[i, j];
                    }
                }

                Tile tile = new Tile();
                tile.paletteIndicies = flippedPaletteIndicies;
                return tile;
            }
        }

        class TileMap
        {
            [Flags]
            public enum FlippingFlags
            {
                None = 0,
                Horizontal = 1 << 0,
                Vertical = 1 << 1,
            }

            public struct MapEntry
            {
                public int tilesetIndex;
                public FlippingFlags flags;
            }

            public MapEntry[,] mapData;
            public Tile[] uniqueTileSet;
            public int paletteIndex = -1;

            public TileMap(Tile[,] rawTiles, Tile[] uniqueTileSet, int paletteIndex)
            {
                mapData = new MapEntry[rawTiles.GetLength(0), rawTiles.GetLength(1)];
                this.uniqueTileSet = uniqueTileSet;
                this.paletteIndex = paletteIndex;

                for (int i = 0; i < rawTiles.GetLength(0); ++i)
                {
                    for (int j = 0; j < rawTiles.GetLength(1); ++j)
                    {
                        Tile tile = rawTiles[i, j];
                        int uTileIndex;
                        FlippingFlags flags = FlippingFlags.None;

                        for (uTileIndex = 0; uTileIndex < uniqueTileSet.Length; ++uTileIndex)
                        {
                            Tile uTile = uniqueTileSet[uTileIndex];

                            var comparisonResult = tile.Compare(uTile);
                            switch (comparisonResult)
                            {
                                case Tile.ComparisonResult.Same:
                                    flags = FlippingFlags.None;
                                    goto TileIndexFound;

                                case Tile.ComparisonResult.FlipH:
                                    flags = FlippingFlags.Horizontal;
                                    goto TileIndexFound;

                                case Tile.ComparisonResult.FlipV:
                                    flags = FlippingFlags.Vertical;
                                    goto TileIndexFound;

                                case Tile.ComparisonResult.FlipHFlipV:
                                    flags = FlippingFlags.Horizontal | FlippingFlags.Vertical;
                                    goto TileIndexFound;

                                default:
                                    break;
                            }
                        }

                    TileIndexFound:

                        if (uTileIndex >= uniqueTileSet.Length)
                            throw new System.Exception(string.Format("Unable to determine unique tile index of tile ({0}, {1})", i, j));

                        mapData[i, j].tilesetIndex = uTileIndex;
                        mapData[i, j].flags = flags;
                    }
                }
            }
        }

        static void FillUniqueTileset(Tile[,] tileMap, List<Tile> tileList)
        {
            for (int y = 0; y < tileMap.GetLength(1); ++y)
            {
                for (int x = 0; x < tileMap.GetLength(0); ++x)
                {
                    Tile tile = tileMap[x, y];
                    bool isUnique = true;

                    foreach(Tile uniqueTile in tileList)
                    {
                        if (tile.Compare(uniqueTile) != Tile.ComparisonResult.Different)
                        {
                            isUnique = false;
                            break;
                        }
                    }

                    if (isUnique)
                    {
                        tileList.Add(tile);
                    }
                }
            }
        }

        Tile[,] BitmapToTileArray(Bitmap bitmap, Color[] palette, int localPaletteIndex = -1)
        {
            int xStart = 0;
            int yStart = 0;
            int width = bitmap.Width;
            int height = bitmap.Height;

            Tile[,] tiles = new Tile[width / TileConfig.c_TILEWIDTH, height / TileConfig.c_TILEHEIGHT];

            List<Color> localPaletteList = new List<Color>();
            if (localPaletteIndex >= 0)
            {
                int localPalStart = PaletteHelper.PALETTE_LENGTH_4BBP * localPaletteIndex;
                if (palette.Length <= localPalStart)
                {
                    throw new Exception("Local palette out of range");
                }

                for (int i = localPalStart; i < Math.Min(localPalStart + PaletteHelper.PALETTE_LENGTH_4BBP, palette.Length); ++i)
                {
                    localPaletteList.Add(palette[i]);
                }
            }

            Color[] localPalette = localPaletteList.Count > 0 ? localPaletteList.ToArray() : palette;

            for (int x = xStart; x < width; x += TileConfig.c_TILEWIDTH)
            {
                for (int y = yStart; y < height; y += TileConfig.c_TILEHEIGHT)
                {
                    Tile tile = GetTile(bitmap, localPalette, x, y);

                    tiles[x / TileConfig.c_TILEWIDTH, y / TileConfig.c_TILEHEIGHT] = tile;
                }
            }

            return tiles;
        }

        static Tile GetTile(Bitmap bitmap, Color[] palette, int x, int y)
        {
            Tile tile = new Tile();

            for (int pixX = x; pixX < x + TileConfig.c_TILEWIDTH; ++pixX)
            {
                for (int pixY = y; pixY < y + TileConfig.c_TILEHEIGHT; ++pixY)
                {
                    Color color = bitmap.GetPixel(pixX, pixY);
                    tile.paletteIndicies[pixX - x, pixY - y] = PaletteHelper.ColorToPaletteIndex(palette, color);
                }
            }

            return tile;
        }
    }
}
