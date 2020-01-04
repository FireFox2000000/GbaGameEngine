using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Drawing;

namespace GbaConversionTools.States
{
    class MenuTilemapConverter
    {
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
    }
}
