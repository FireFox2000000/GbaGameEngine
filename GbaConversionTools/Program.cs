using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Drawing;
using System.Drawing.Imaging;
using System.Numerics;

namespace GbaConversionTools
{
    class Program
    {
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

        static void AnyKeyToContinue()
        {
            Console.WriteLine("Press any key to continue");
            Console.ReadKey();
        }

        static void Main(string[] args)
        {
            // Todo, remove hardcoded filenames
            //string inputPath = "Shantae_Idle.png";

            Console.WriteLine("Drag a sprite file to convert");
            string inputPath = Console.ReadLine().Trim('"');

            Bitmap bitmap = new Bitmap(inputPath);
            string outputPath = Path.GetFileNameWithoutExtension(inputPath) + ".h";

            Console.WriteLine("Select slicing method:");
            Console.WriteLine("1. Grid slice");

            try
            {
                Tools.SpriteConverter.SliceCoordinate[] sliceCoordinates = GenerateGridSlice(bitmap);

                ///////////////////////////////////////

                Tools.SpriteConverter spriteConverter = new Tools.SpriteConverter();
                spriteConverter.Convert(inputPath, outputPath, bitmap, sliceCoordinates);
            }
            catch (Exception e)
            {
                Console.WriteLine("Error occured: " + e.Message);
            }

            AnyKeyToContinue();
        }

        static bool IsValidPixelCombination(Vector2 pixelSize)
        {
            foreach(Vector2 validCombination in validPixelCombinations)
            {
                if (pixelSize == validCombination)
                    return true;
            }

            return false;
        }

        static Tools.SpriteConverter.SliceCoordinate[] GenerateGridSlice(Bitmap bitmap)
        {
            int rows, columns;
            int spriteWidth, spriteHeight;

            const string intParseErrorMsg = "Error reading input, make sure you are entering an integer value";
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

            Tools.SpriteConverter.SliceCoordinate[] sliceCoordinates = new Tools.SpriteConverter.SliceCoordinate[rows * columns];
            for (int col = 0; col < columns; ++col)
            {
                for (int row = 0; row < rows; ++row)
                {
                    int index = row * columns + col;

                    sliceCoordinates[index].x = index * col;
                    sliceCoordinates[index].y = index * row;

                    sliceCoordinates[index].width = spriteWidth;
                    sliceCoordinates[index].height = spriteHeight;
                }
            }

            return sliceCoordinates;
        }

        static Tools.SpriteConverter.SliceCoordinate[] UniformSize(Bitmap bitmap)
        {
            Console.WriteLine("Sprite width: ");

            Console.WriteLine("Sprite height: ");

            return new Tools.SpriteConverter.SliceCoordinate[0];
        }
    }
}
