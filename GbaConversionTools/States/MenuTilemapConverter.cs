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
    class MenuTilemapConverter : IMenuState
    {
        const string intParseErrorMsg = "Error reading input, make sure you are entering an integer value";

        public void Enter()
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

        void WalkConverterOptions()
        {
            Console.WriteLine("Drag an image file to convert, or drag a folder path to make a tilemap set");

            List<string> files = new List<string>();
            string inputPath = Console.ReadLine().Trim('"');
            {
                if (Directory.Exists(inputPath))
                {
                    files.AddRange(Directory.GetFiles(inputPath));
                }
                else if (File.Exists(inputPath))
                {
                    files.Add(inputPath);
                }
            }

            List<Bitmap> bitmaps = new List<Bitmap>();

            foreach (string filePath in files)
            {
                try
                {
                    Bitmap bitmap = new Bitmap(filePath);

                    // Validate input
                    if (!Tools.TilemapConverter.IsValidPixelCombination(new Vector2(bitmap.Width, bitmap.Height)))
                    {
                        throw new Exception("File not a valid pixel combination;");
                    }

                    bitmaps.Add(bitmap);
                }
                catch (Exception e)
                {
                    Console.WriteLine(string.Format("Unable to process file {0} as a bitmap: {1}"), e.Message, e.Message);
                }
            }

            if (bitmaps.Count > 0)
            {
                try
                {
                    string outputPath = Path.Combine(FileManager.Instance.TilemapPath, Path.GetFileName(Path.ChangeExtension(inputPath, ".cpp")));

                    Tools.TilemapConverter converter = new Tools.TilemapConverter();
                    converter.Convert(inputPath, outputPath, bitmaps);
                }
                catch (Exception e)
                {
                    Console.WriteLine("Error occured: " + e.Message);
                }
            }
        }
    }
}
