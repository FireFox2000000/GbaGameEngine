//#define FORCE_DYNAMIC_RENDERING

using System;
using System.Text;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Collections.Generic;
using System.Diagnostics;
using System.Numerics;

namespace GbaConversionTools.Tools
{
    // Currently only supports making a single tilemap set per scene. Todo, fill ui and other global tilemap palettes from the back slots to use in all scenes.
    // Convert multiple images together to make a proper scene out of them
    class TilemapConverter
    {
        //const string compressionComment = "// Bit0 - 3   Data size in bit units(normally 4 or 8). May be reserved/unused for other compression types \n" + namespaceTabs +
        // "// Bit4-7   Compressed type \n" + namespaceTabs +
        // "// Bit8-31  Unused, generated in-game as 24bit size of decompressed data in bytes, probably \n";

        const int MAX_UNIQUE_TILES = 1024;
        const int GBA_MAP_TILE_WIDTH = 32;
        const int GBA_MAP_TILE_HEIGHT = 32;

        // Valid regular tilemap sizes are 32 or 64
        const int REG_TILEMAP_MIN_SIZE = 32;
        const int REG_TILEMAP_MAX_SIZE = 64;

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

        /// <param name="inputPath">Input file path, namespace name is derived from this</param>
        /// <param name="outputPath">Output cpp file</param>
        /// <param name="bitmaps">Image files to create tilemap from</param>
        /// <param name="paletteBankIndexOffset">Allows us to force things like UI palettes into the back of the palette bank</param>
        public void Convert(string inputPath, string outputPath, Bitmap inputBitmap, byte paletteBankIndexOffset = 0)
        {
            List<Bitmap> bitmaps = new List<Bitmap>();
            bitmaps.Add(inputBitmap);

            Convert(inputPath, outputPath, bitmaps, paletteBankIndexOffset);
        }

        /// <param name="inputPath">Input file path, namespace name is derived from this</param>
        /// <param name="outputPath">Output cpp file</param>
        /// <param name="bitmaps">List of image files to map tilemaps from</param>
        /// <param name="paletteBankIndexOffset">Allows us to force things like UI palettes into the back of the palette bank</param>
        public void Convert(string inputPath, string outputPath, IList<Bitmap> bitmaps, byte paletteBankIndexOffset = 0)
        {
            CppWriter cppWriter = new CppWriter(Path.GetFileName(Path.GetFileNameWithoutExtension(inputPath)), outputPath);

            Console.WriteLine("Processing master palette");

            Color[] masterPalette;
            Tile[] masterTileSet;
            TileMap[] tileMapList;

            GenerateTileMaps(bitmaps, paletteBankIndexOffset, out masterPalette, out masterTileSet, out tileMapList);
            GBAMapData gbaMapData = GenerateMapData(tileMapList, paletteBankIndexOffset);

            const int maxMaps = sizeof(byte) * 8;
            if (gbaMapData.mapIsDynamic.Count >= maxMaps)
            {
                // Can't store this in the current bitmask size
                throw new Exception(string.Format("Too many tilemaps found. Maps = {0}, max is {1}. Consider increasing the bitmask of \"mapIsDynamicBitMask\".", gbaMapData.mapIsDynamic.Count, maxMaps));
            }

            Console.WriteLine("Total unique tiles = " + masterTileSet.Length);
            Console.WriteLine("Processing tilemaps");

            Compression.CompressionType compressionType = Compression.CompressionType.BitPacked;
            uint destBpp = CalculateDestBitsPerPixel(masterTileSet);

            List<UInt32> tileSetData = GenerateTileSetData(masterTileSet, compressionType, destBpp);

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

                byte mapIsDynamicBitMask = BoolListToBitmaskU8(gbaMapData.mapIsDynamic);
                cppWriter.Write(mapIsDynamicBitMask);

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

            Console.WriteLine("Tilemap \"" + outputPath + "\" successfully converted");

            cppWriter.Finalise();
        }

        static byte BoolListToBitmaskU8(IList<bool> list)
        {
            byte bitmask = 0;
            for (int i = 0; i < list.Count; ++i)
            {
                if (list[i])
                {
                    bitmask |= (byte)(1 << i);
                }
            }

            return bitmask;
        }

        public static List<UInt32> GenerateTileSetData(Tile[] tiles, Compression.CompressionType compressionType, uint destBpp)
        {
            UInt32 compressionTypeSize = Compression.ToGBACompressionHeader(compressionType, destBpp);

            List<UInt32> allCompressedIndicies = new List<UInt32>();

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

                allCompressedIndicies.AddRange(compressedIndicies);
            } 

            return allCompressedIndicies;
        }

        public static uint CalculateDestBitsPerPixel(Tile[] masterTileSet)
        {
            uint destBpp = 4;

            foreach (Tile tile in masterTileSet)
            {
                foreach (int index in tile.paletteIndicies)
                {
                    if (index >= PaletteHelper.PALETTE_LENGTH_4BBP)
                    {
                        // We know that we have no choice but to write in 8bpp format
                        destBpp = 8;
                        return destBpp;
                    }
                }
            }

            return destBpp;
        }

        // Groups of 16 colours
        static int Find4bbpPaletteIndex(IList<Color> paletteList, IList<Color> desiredPalette)
        {
            int PaletteSize = PaletteHelper.PALETTE_LENGTH_4BBP;
            int paletteIndex = 0;

            int startIndex = paletteIndex * PaletteSize;
            int endIndex = Math.Min(startIndex + PaletteSize, paletteList.Count);

            while (startIndex < paletteList.Count)
            {
                bool coloursFound = true;
                foreach (Color col in desiredPalette)
                {
                    bool colourFound = false;
                    for (int i = startIndex; i < endIndex; ++i)
                    {
                        if (paletteList[i] == col)
                        {
                            colourFound = true;
                            break;
                        }
                    }

                    coloursFound &= colourFound;
                }

                if (coloursFound)
                {
                    return paletteIndex;
                }

                ++paletteIndex;
                startIndex = paletteIndex * PaletteSize;
                endIndex = Math.Min(startIndex + PaletteSize, paletteList.Count);
            }

            return -1;
        }

        static int Get4bbpPaletteIndexForSize(IList<Color> palette)
        {
            return palette.Count / PaletteHelper.PALETTE_LENGTH_4BBP;
        }

        public static void GenerateTileMaps(
            IList<Bitmap> bitmaps
            , byte paletteBankIndexOffset
            , out Color[] masterPalette
            , out Tile[] masterTileSet
            , out TileMap[] tileMaps
            , bool skipSort = false
            )
        {
            List<ProcessedPaletteContainer> bitmapPalettes = new List<ProcessedPaletteContainer>();

            // Validate bitmaps and collect palettes
            for (int bitmapIndex = 0; bitmapIndex < bitmaps.Count; ++bitmapIndex)
            {
                Bitmap bitmap = bitmaps[bitmapIndex];

                Size size = bitmap.Size;

                if (size.Width % TileConfig.TileWidth != 0 || size.Height % TileConfig.TileHeight != 0)
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

            if (!skipSort)
            {
                // Sort bitmap palette. Anything more than 16 colours should be grouped next to each other. Less than 16 should always be a full 16.
                bitmapPalettes.Sort(new ProcessedPaletteComparer());
            }

            // Get palettes of each image, merge into a master palette
            List<Color> masterPaletteList = new List<Color>();
            List<ProcessedBitmapContainer> processedBitmapList = new List<ProcessedBitmapContainer>();
            {
                Color transparencyColour = Color.FromArgb(0);

                for (int i = 0; i < bitmapPalettes.Count; ++i)
                {
                    var bp = bitmapPalettes[i];

                    Debug.Assert(bp.palette[0] == transparencyColour);

                    bool pal4bbp = bp.palette.Length <= PaletteHelper.PALETTE_LENGTH_4BBP;
                    int paletteIndex = pal4bbp ? Find4bbpPaletteIndex(masterPaletteList, bp.palette) : -1;

                    if (pal4bbp && paletteIndex < 0)    // Add the new palette in
                    {
                        // Make sure our 4bbp palette is aligned properly, every set of 16 colour palettes starts with pure transparency
                        while (masterPaletteList.Count % PaletteHelper.PALETTE_LENGTH_4BBP != 0)
                        {
                            masterPaletteList.Add(transparencyColour);
                        }

                        // TODO
                        // If 2 bitmap palettes can fit in the same GBA palette, merge them into one. 
                        paletteIndex = Get4bbpPaletteIndexForSize(masterPaletteList);

                        masterPaletteList.AddRange(bp.palette);
                    }

                    if (!pal4bbp)
                    {
                        // Only add colour into the master palette if it's not already in there.
                        foreach (Color col in bp.palette)
                        {
                            if (!masterPaletteList.Contains(col))
                            {
                                masterPaletteList.Add(col);
                            }
                        }
                    }

                    if ((paletteIndex + paletteBankIndexOffset) >= PaletteHelper.MAX_PALETTE_INDEX)
                    {
                        throw new Exception(string.Format("Palette index {0} with palette bank offset {1} is not valid for map data", paletteIndex, paletteBankIndexOffset));
                    }

                    ProcessedBitmapContainer pbc = new ProcessedBitmapContainer();
                    pbc.bitmapIndex = bp.bitmapIndex;
                    pbc.paletteIndex = paletteIndex;
                    pbc.bitmap = bitmaps[bp.bitmapIndex];

                    processedBitmapList.Add(pbc);
                }
            }

            masterPalette = masterPaletteList.ToArray();

            Console.WriteLine("Total colours found = " + masterPalette.Length);

            if (masterPalette.Length >= PaletteHelper.MAX_PALETTE_LENGTH)
            {
                throw new Exception(string.Format("Master palette has too many colours ({0}/{1})", masterPalette.Length, PaletteHelper.MAX_PALETTE_LENGTH));
            }

            Console.WriteLine("Processing master tileset");

            // Get all unique tiles sorted via palette indicies. Need to check if local palette was greater than 16 or not
            List<Tile> uniqueTileSetList = new List<Tile>();
            foreach (ProcessedBitmapContainer pbc in processedBitmapList)
            {
                Tile[,] rawTileMap = BitmapToTileArray(pbc.bitmap, masterPalette, pbc.paletteIndex);
                pbc.rawTileMap = rawTileMap;

                FillUniqueTileset(rawTileMap, uniqueTileSetList);
            }

            masterTileSet = uniqueTileSetList.ToArray();

            // Generate tilemaps from master list. Remember palette index, but we should be able to forget about it. We can overlap tiles that are the same but different colours.
            List<TileMap> tileMapList = new List<TileMap>();
            foreach (ProcessedBitmapContainer pbc in processedBitmapList)
            {
                TileMap tilemap = new TileMap(pbc.rawTileMap, masterTileSet, pbc.paletteIndex);
                tileMapList.Add(tilemap);
            }

            tileMaps = tileMapList.ToArray();
        }

        public struct GBAMapData
        {
            public GBAScreenEntry[] screenEntries;
            public List<bool> mapIsDynamic;
            public List<int> widthLists;
            public List<int> heightLists;
        }

        public static GBAMapData GenerateMapData(IList<TileMap> tilemaps, int paletteBankIndexOffset)
        {
            List<GBAScreenEntry> seList = new List<GBAScreenEntry>();
            List<int> mapStartOffsets = new List<int>();

            // Write map sizes
            List<int> widthLists = new List<int>();
            List<int> heightLists = new List<int>();

            List<bool> mapIsDynamic = new List<bool>();

            for (int tilemapIndex = 0; tilemapIndex < tilemaps.Count; ++tilemapIndex)
            {
                var tilemap = tilemaps[tilemapIndex];

                int paletteIndex = tilemap.paletteIndex + paletteBankIndexOffset;
                if (paletteIndex >= PaletteHelper.MAX_PALETTE_INDEX)
                {
                    throw new Exception(string.Format("Palette index {0} is not valid for map data", paletteIndex));
                }

                int width = tilemap.mapData.GetLength(0);
                int height = tilemap.mapData.GetLength(1);

                widthLists.Add(width);
                heightLists.Add(height);

                mapStartOffsets.Add(seList.Count);

                bool widthValidForNesting = width % GBA_MAP_TILE_WIDTH == 0 && width >= REG_TILEMAP_MIN_SIZE && width <= REG_TILEMAP_MAX_SIZE;
                bool heightValidForNesting = height % GBA_MAP_TILE_HEIGHT == 0 && height >= REG_TILEMAP_MIN_SIZE && height <= REG_TILEMAP_MAX_SIZE;

#if FORCE_DYNAMIC_RENDERING
                bool performGbaNesting = false;
#else
                bool performGbaNesting = widthValidForNesting && heightValidForNesting; // Disable for dynamic maps
#endif
                mapIsDynamic.Add(!performGbaNesting);

                if (performGbaNesting)
                {
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
                                        screenEntry.SetPalIndex(paletteIndex);  // May be wrong when merging the palette

                                    seList.Add(screenEntry);
                                }
                            }
                        }
                    }
                }
                else
                {
                    for (int mapY = 0; mapY < tilemap.mapData.GetLength(1); ++mapY)
                    {            
                        for (int mapX = 0; mapX < tilemap.mapData.GetLength(0); ++mapX)
                        {
                            var currentMapData = tilemap.mapData[mapX, mapY];

                            GBAScreenEntry screenEntry = new GBAScreenEntry();
                            screenEntry.SetTileIndex(currentMapData.tilesetIndex);

                            if ((currentMapData.flags & TileMap.FlippingFlags.Horizontal) != 0)
                                screenEntry.SetHFlipFlag();

                            if ((currentMapData.flags & TileMap.FlippingFlags.Vertical) != 0)
                                screenEntry.SetVFlipFlag();

                            if (tilemap.paletteIndex >= 0)
                                screenEntry.SetPalIndex(paletteIndex);

                            seList.Add(screenEntry);
                        }
                    }
                }

                Console.WriteLine("Processed tilemap {0}:", tilemapIndex);
                if (performGbaNesting)
                {
                    Console.WriteLine("\tStatic/GBA nested map");
                }
                else
                {
                    Console.WriteLine("\tDynamically rendered map");
                }
            }

            GBAScreenEntry[] mapEntries = seList.ToArray();

            GBAMapData generatedMapData;
            generatedMapData.screenEntries = mapEntries;
            generatedMapData.mapIsDynamic = mapIsDynamic;
            generatedMapData.widthLists = widthLists;
            generatedMapData.heightLists = heightLists;

            return generatedMapData;
        }

        public class GBAScreenEntry
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

        public class Tile
        {
            public enum ComparisonResult
            {
                Different,
                Same,
                FlipH,
                FlipV,
                FlipHFlipV,
            }

            public int[,] paletteIndicies = new int[TileConfig.TileWidth, TileConfig.TileHeight];

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
                int[,] flippedPaletteIndicies = new int[TileConfig.TileWidth, TileConfig.TileHeight];

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
                int[,] flippedPaletteIndicies = new int[TileConfig.TileWidth, TileConfig.TileHeight];

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

        public class TileMap
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

        static Tile[,] BitmapToTileArray(Bitmap bitmap, Color[] palette, int localPaletteIndex = -1)
        {
            int xStart = 0;
            int yStart = 0;
            int width = bitmap.Width;
            int height = bitmap.Height;

            Tile[,] tiles = new Tile[width / TileConfig.TileWidth, height / TileConfig.TileHeight];

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

            for (int x = xStart; x < width; x += TileConfig.TileWidth)
            {
                for (int y = yStart; y < height; y += TileConfig.TileHeight)
                {
                    Tile tile = GetTile(bitmap, localPalette, x, y);

                    tiles[x / TileConfig.TileWidth, y / TileConfig.TileHeight] = tile;
                }
            }

            return tiles;
        }

        static Tile GetTile(Bitmap bitmap, Color[] palette, int x, int y)
        {
            Tile tile = new Tile();

            for (int pixX = x; pixX < x + TileConfig.TileWidth; ++pixX)
            {
                for (int pixY = y; pixY < y + TileConfig.TileHeight; ++pixY)
                {
                    Color color = bitmap.GetPixel(pixX, pixY);
                    tile.paletteIndicies[pixX - x, pixY - y] = PaletteHelper.ColorToPaletteIndex(palette, color);
                }
            }

            return tile;
        }

        const int PIXELS_PER_TILE_X = 8;
        const int PIXELS_PER_TILE_Y = 8;

        static Vector2[] ValidNestablePixelCombinations = new Vector2[]
        {
                new Vector2(32 * PIXELS_PER_TILE_X, 32 * PIXELS_PER_TILE_Y),
                new Vector2(64 * PIXELS_PER_TILE_X, 32 * PIXELS_PER_TILE_Y),
                new Vector2(32 * PIXELS_PER_TILE_X, 64 * PIXELS_PER_TILE_Y),
                new Vector2(64 * PIXELS_PER_TILE_X, 64 * PIXELS_PER_TILE_Y),
        };

        public static bool IsNestableSize(Vector2 pixelSize)
        {
            foreach (Vector2 validCombination in ValidNestablePixelCombinations)
            {
                if (pixelSize == validCombination)
                    return true;
            }

            return false;
        }

        public static bool IsValidPixelCombination(Vector2 pixelSize)
        {
            if (pixelSize.X % TileConfig.TileWidth != 0)
            {
                Console.WriteLine(string.Format("Image width ({0}) was invalid for the platform. Must be a multiple of {1} pixels.", pixelSize.X, TileConfig.TileWidth));
                return false;
            }

            if (pixelSize.Y % TileConfig.TileHeight != 0)
            {
                Console.WriteLine(string.Format("Image height ({0}) was invalid for the platform. Must be a multiple of {1} pixels.", pixelSize.Y, TileConfig.TileHeight));
                return false;
            }

            if (IsNestableSize(pixelSize))
            {
                return true;
            }

            // Check if it's a dynamic map
            if (pixelSize.X >= REG_TILEMAP_MIN_SIZE * TileConfig.TileWidth && pixelSize.Y >= REG_TILEMAP_MIN_SIZE * TileConfig.TileHeight)
            {
                return true;
            }

            Console.WriteLine(string.Format("Image size ({0}, {1}) was invalid for the platform", pixelSize.X, pixelSize.Y));
            Console.WriteLine("Dimensions must be a multiple of 8 and be at least 256 pixels wide and 256 pixels tall.");

            return false;
        }
    }
}
