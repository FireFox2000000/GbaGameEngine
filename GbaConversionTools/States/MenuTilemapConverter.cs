using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Drawing;
using System.Numerics;

namespace GbaConversionTools.States
{
    static class MenuTilemapConverter
    {
        const int PIXELS_PER_TILE_X = 8;
        const int PIXELS_PER_TILE_Y = 8;

        static Vector2[] validPixelCombinations = new Vector2[]
        {
                new Vector2(32 * PIXELS_PER_TILE_X, 32 * PIXELS_PER_TILE_Y),
                new Vector2(64 * PIXELS_PER_TILE_X, 32 * PIXELS_PER_TILE_Y),
                new Vector2(32 * PIXELS_PER_TILE_X, 64 * PIXELS_PER_TILE_Y),
                new Vector2(64 * PIXELS_PER_TILE_X, 64 * PIXELS_PER_TILE_Y),
        };

        const string intParseErrorMsg = "Error reading input, make sure you are entering an integer value";

        public static void Enter()
        {
            while (true)
            {
                WalkConverterOptions();

                Console.WriteLine("Convert another tilemap? (Y/N)");
                string inputPath = Console.ReadLine().ToLower();

                if (inputPath.Length <= 0 || inputPath[0] != 'y')
                {
                    break;
                }
            }
        }

        static void WalkConverterOptions()
        {
            Console.WriteLine("Drag an image file to convert");
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

            // Validate input
            if (!IsValidPixelCombination(new Vector2(bitmap.Width, bitmap.Height)))
            {
                Console.WriteLine(string.Format("Image size ({0}, {1}) was invalid for the platform", bitmap.Width, bitmap.Height));
                Console.WriteLine("Valid sizes: ");
                foreach(Vector2 size in validPixelCombinations)
                {
                    Console.WriteLine(string.Format("\t({0}, {1})", size.X, size.Y));
                }
                return;
            }

            try
            {
                string outputPath = Path.ChangeExtension(inputPath, ".cpp");

                Tools.TilemapConverter converter = new Tools.TilemapConverter();
                converter.Convert(inputPath, outputPath, bitmap);
            }
            catch (Exception e)
            {
                Console.WriteLine("Error occured: " + e.Message);
            }
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
