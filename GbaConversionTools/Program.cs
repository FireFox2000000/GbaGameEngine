using System;
using System.Collections.Generic;
using GbaConversionTools.States;

namespace GbaConversionTools
{
    class Program
    {
        const string intParseErrorMsg = "Error reading input, make sure you are entering an integer value";

        static void AnyKeyToContinue()
        {
            Console.WriteLine("Press any key to continue");
            Console.ReadKey();
        }

        static void Main(string[] args)
        {
            Console.WriteLine("Select converter:");
            Console.WriteLine("1. Sprite");
            Console.WriteLine("2. Tilemap");
            Console.WriteLine("3. Audio");

            int menuOption;
            while (!Int32.TryParse(Console.ReadLine(), out menuOption))
            {
                Console.WriteLine(intParseErrorMsg);
                continue;
            }

            while (true)
            {
                IMenuState menuState = null;

                switch (menuOption)
                {
                    case 1:
                        {
                            menuState = new MenuSpriteConverter();
                            break;
                        }
                    case 2:
                        {
                            menuState = new MenuTilemapConverter();
                            break;
                        }
                    case 3:
                        {
                            menuState = new MenuAudioConverter();
                            break;
                        }
                    default:
                        Console.WriteLine("Invalid menu option");
                        continue;
                }

                menuState.Enter();

                break;
            }

            AnyKeyToContinue();
        }

        
    }
}
