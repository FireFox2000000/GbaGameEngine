using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Drawing;
using System.Drawing.Imaging;
using System.Numerics;
using Newtonsoft.Json;

namespace GbaConversionTools.States
{
    static class MenuSpriteConverter
    {
        const string intParseErrorMsg = "Error reading input, make sure you are entering an integer value";

        const int PIXELS_PER_TILE_X = 8;
        const int PIXELS_PER_TILE_Y = 8;

        static Vector2[] validPixelCombinations = new Vector2[]
        {
                new Vector2(1 * PIXELS_PER_TILE_X, 1 * PIXELS_PER_TILE_Y),
                new Vector2(2 * PIXELS_PER_TILE_X, 2 * PIXELS_PER_TILE_Y),
                new Vector2(4 * PIXELS_PER_TILE_X, 4 * PIXELS_PER_TILE_Y),
                new Vector2(8 * PIXELS_PER_TILE_X, 8 * PIXELS_PER_TILE_Y),

                new Vector2(2 * PIXELS_PER_TILE_X, 1 * PIXELS_PER_TILE_Y),
                new Vector2(4 * PIXELS_PER_TILE_X, 1 * PIXELS_PER_TILE_Y),
                new Vector2(4 * PIXELS_PER_TILE_X, 2 * PIXELS_PER_TILE_Y),
                new Vector2(8 * PIXELS_PER_TILE_X, 4 * PIXELS_PER_TILE_Y),

                new Vector2(1 * PIXELS_PER_TILE_X, 2 * PIXELS_PER_TILE_Y),
                new Vector2(1 * PIXELS_PER_TILE_X, 4 * PIXELS_PER_TILE_Y),
                new Vector2(2 * PIXELS_PER_TILE_X, 4 * PIXELS_PER_TILE_Y),
                new Vector2(4 * PIXELS_PER_TILE_X, 8 * PIXELS_PER_TILE_Y),
        };

        public static void Enter()
        {
            Console.WriteLine("Drag a sprite file to convert");
            string inputPath = Console.ReadLine().Trim('"');

            if (!File.Exists(inputPath))
            {
                Console.WriteLine(string.Format("Unable to find file {0}", inputPath));
                return;
            }


            Bitmap bitmap;

            try
            {
                bitmap = new Bitmap(inputPath);
            }
            catch (Exception e)
            {
                Console.WriteLine("Unable to process file as a bitmap: " + e.Message);
                return;
            }

            try
            {
                string outputPath = Path.ChangeExtension(inputPath, ".cpp");

                int uvProcessingOption;
                Tools.SpriteConverter.UVs[] sliceCoordinates;

                while (true)
                {
                    Console.WriteLine("Select slicing method:");
                    Console.WriteLine("1. Grid slice");
                    Console.WriteLine("2. From Json UVs");

                    while (!Int32.TryParse(Console.ReadLine(), out uvProcessingOption))
                    {
                        Console.WriteLine(intParseErrorMsg);
                        continue;
                    }

                    switch (uvProcessingOption)
                    {
                        case 1:
                            // Read user input
                            sliceCoordinates = PerformGridSlice(bitmap);
                            break;
                        case 2:
                            // Load from json file
                            sliceCoordinates = LoadJsonUVs();
                            break;
                        default:
                            Console.WriteLine("Invalid slicing option");
                            continue;
                    }

                    break;
                }

                ///////////////////////////////////////

                Tools.SpriteConverter spriteConverter = new Tools.SpriteConverter();
                spriteConverter.Convert(inputPath, outputPath, bitmap, sliceCoordinates);

                string jsonOutputPath = Path.GetDirectoryName(inputPath) + Path.DirectorySeparatorChar + Path.GetFileNameWithoutExtension(inputPath) + "_uvs.json";
                Console.WriteLine("Saving UV coordiates to " + jsonOutputPath);

                string uvJsonData = JsonConvert.SerializeObject(sliceCoordinates, Formatting.Indented);
                File.WriteAllText(jsonOutputPath, uvJsonData);
            }
            catch (Exception e)
            {
                Console.WriteLine("Error occured: " + e.Message);
            }
        }

        static Tools.SpriteConverter.UVs[] PerformGridSlice(Bitmap bitmap)
        {
            int rows, columns;
            int spriteWidth, spriteHeight;


            int width = bitmap.Width, height = bitmap.Height;

            while (true)
            {
                Console.WriteLine("Enter the numbers of rows: ");
                while (!Int32.TryParse(Console.ReadLine(), out rows))
                    Console.WriteLine(intParseErrorMsg);

                Console.WriteLine("Enter the number of columns: ");
                while (!Int32.TryParse(Console.ReadLine(), out columns))
                    Console.WriteLine(intParseErrorMsg);

                float spriteWidthF = width / columns;
                float spriteHeightF = height / rows;

                Console.WriteLine("Individual sprite size is ({0}, {1})", spriteWidthF, spriteHeightF);

                // Validate input
                if (!IsValidPixelCombination(new Vector2(spriteWidthF, spriteHeightF)))
                {
                    Console.WriteLine("Sprite size was invalid for the platform\n");
                    continue;
                }

                spriteWidth = (int)spriteWidthF;
                spriteHeight = (int)spriteHeightF;

                break;
            }

            Tools.SpriteConverter.UVs[] sliceCoordinates = new Tools.SpriteConverter.UVs[rows * columns];
            for (int col = 0; col < columns; ++col)
            {
                for (int row = 0; row < rows; ++row)
                {
                    int index = row * columns + col;

                    sliceCoordinates[index].x = spriteWidth * col;
                    sliceCoordinates[index].y = spriteHeight * row;

                    sliceCoordinates[index].width = spriteWidth;
                    sliceCoordinates[index].height = spriteHeight;
                }
            }

            return sliceCoordinates;
        }

        static Tools.SpriteConverter.UVs[] LoadJsonUVs()
        {
            string inputPath = string.Empty;

            while (true)
            {
                Console.WriteLine("Enter uvs file");
                inputPath = Console.ReadLine().Trim('"');

                if (!File.Exists(inputPath))
                {
                    Console.WriteLine("Unable to locate file");
                    continue;
                }

                break;
            }

            Tools.SpriteConverter.UVs[] uvs = JsonConvert.DeserializeObject<Tools.SpriteConverter.UVs[]>(File.ReadAllText(inputPath));

            return uvs;
        }

        static Tools.SpriteConverter.UVs[] UniformSize(Bitmap bitmap)
        {
            Console.WriteLine("Sprite width: ");

            Console.WriteLine("Sprite height: ");

            return new Tools.SpriteConverter.UVs[0];
        }

        static bool IsValidPixelCombination(Vector2 pixelSize)
        {
            foreach (Vector2 validCombination in validPixelCombinations)
            {
                if (pixelSize == validCombination)
                    return true;
            }

            return false;
        }
    }
}
