using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Drawing;
using System.Drawing.Imaging;

namespace GbaConversionTools
{
    class Program
    {
        static void AnyKeyToContinue()
        {
            Console.WriteLine("Press any key to continue");
            Console.ReadKey();
        }

        static void Main(string[] args)
        {
            // Todo, remove hardcoded filenames
            string inputPath = "Shantae.bmp";

            try
            {
                Tools.SpriteConverter spriteConverter = new Tools.SpriteConverter();
                spriteConverter.Convert(inputPath);
            }
            catch (Exception e)
            {
                Console.WriteLine("Error occured: " + e.Message);
            }

            AnyKeyToContinue();
        }
    }
}
