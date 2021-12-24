using System;
using System.Collections.Generic;
using GbaConversionTools.States;

namespace GbaConversionTools
{
    class Program
    {
        const string intParseErrorMsg = "Error reading input, make sure you are entering an integer value";

        public delegate IMenuState CreateMenuFn();

        struct ConverterOption
        {
            public string menuLabel;
            public CreateMenuFn createMenuStateFn;
        }

        static void AnyKeyToContinue()
        {
            Console.WriteLine("Press any key to continue");
            Console.ReadKey();
        }

        static void Main(string[] args)
        {
            const int MenuIndexOffset = 1;      // Start menu off at 1 rather than 0

            ConverterOption[] menuOptions = new ConverterOption[]
            {
                new ConverterOption { menuLabel = "Sprite", createMenuStateFn = () => { return new MenuSpriteConverter(); } },
                new ConverterOption { menuLabel = "Tilemap", createMenuStateFn = () => { return new MenuTilemapConverter(); } },
                new ConverterOption { menuLabel = "Audio", createMenuStateFn = () => { return new MenuAudioConverter(); } },
                new ConverterOption { menuLabel = "Ui Atlus", createMenuStateFn = () => { return new MenuUiAtlusConverter(); } },
            };

            Console.WriteLine("Select converter:");
            for (int i = 0; i < menuOptions.Length; ++i)
            {
                Console.WriteLine(string.Format("{0}. {1}", i + MenuIndexOffset, menuOptions[i].menuLabel));
            }

            int menuOption;
            while (!Int32.TryParse(Console.ReadLine(), out menuOption))
            {
                Console.WriteLine(intParseErrorMsg);
                continue;
            }

            while (true)
            {
                if (menuOption >= 0 + MenuIndexOffset && menuOption < menuOptions.Length + MenuIndexOffset)
                {
                    IMenuState menu = menuOptions[menuOption - MenuIndexOffset].createMenuStateFn();
                    menu.Enter();
                    break;
                }
                else
                {
                    Console.WriteLine("Invalid menu option");
                    continue;
                }
            }

            AnyKeyToContinue();
        }  
    }
}
