using System;
using System.Collections.Generic;
using GbaConversionTools.States;

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
            MenuSpriteConverter.Enter();

            AnyKeyToContinue();
        }

        
    }
}
