using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace GbaConversionTools.States
{
    class MenuAudioConverter : IMenuState
    {
        public void Enter()
        {
            while (true)
            {
                WalkConverterOptions();

                Console.WriteLine("Convert another audio file? (Y/N)");
                string inputPath = Console.ReadLine().ToLower();

                if (inputPath.Length <= 0 || inputPath[0] != 'y')
                {
                    break;
                }
            }
        }

        void WalkConverterOptions()
        {
            Console.WriteLine("Drag an audio file to convert");
            string inputPath = Console.ReadLine().Trim('"');

            if (!File.Exists(inputPath))
            {
                Console.WriteLine(string.Format("Unable to find file {0}", inputPath));
                return;
            }

            try
            {
                string outputPath = Path.ChangeExtension(inputPath, ".cpp");

                Tools.AudioConverter converter = new Tools.AudioConverter();
                converter.Convert(inputPath, outputPath);
            }
            catch (Exception e)
            {
                Console.WriteLine("Error occured: " + e.Message);
            }
        }
    }
}
