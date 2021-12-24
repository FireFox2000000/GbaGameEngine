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
    internal class MenuUiAtlusConverter : IMenuState
    {
        public void Enter()
        {
            WalkConverterOptions();
        }

        struct UiAtlusUvs
        {
            public string name;
            public Tools.SpriteConverter.UVs uvs;
        }

        void WalkConverterOptions()
        {
            const int PaletteBankIndexOffset = 14;  // TODO, should probably specify this in the json file

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
                string jsonOutputPath = Path.GetDirectoryName(inputPath) + Path.DirectorySeparatorChar + Path.GetFileNameWithoutExtension(inputPath) + ".json";
                bool uvsJsonExists = File.Exists(jsonOutputPath);

                UiAtlusUvs[] sliceCoordinates;
                if (uvsJsonExists)
                {
                    // Make a list of new bitmaps that are cloned from sections of the original image. 

                    sliceCoordinates = JsonConvert.DeserializeObject<UiAtlusUvs[]>(File.ReadAllText(jsonOutputPath));

                    var bitmaps = new List<Tools.UiAtlusConverter.UiAtlusBitmap>();

                    foreach (UiAtlusUvs atlusSlice in sliceCoordinates)
                    {
                        Rectangle rect = new Rectangle(atlusSlice.uvs.x, atlusSlice.uvs.y, atlusSlice.uvs.width, atlusSlice.uvs.height);
                        bitmaps.Add(new Tools.UiAtlusConverter.UiAtlusBitmap() { name = atlusSlice.name, bitmap = bitmap.Clone(rect, bitmap.PixelFormat) } );
                    }

                    Tools.UiAtlusConverter converter = new Tools.UiAtlusConverter();
                    converter.Convert(inputPath, outputPath, bitmaps, PaletteBankIndexOffset);
                }
                else
                {
                    Console.WriteLine(string.Format("No uvs defined for UI atlus image at {0}. Creating skeleton json file.", inputPath));

                    sliceCoordinates = new UiAtlusUvs[1]
                    {
                        new UiAtlusUvs { name = "UvName1", uvs = new Tools.SpriteConverter.UVs{ x = 0, y = 0, width = 8, height = 8 } },
                    };

                    string uvJsonData = JsonConvert.SerializeObject(sliceCoordinates, Formatting.Indented);
                    File.WriteAllText(jsonOutputPath, uvJsonData);
                }
            }
            catch (Exception e)
            {
                Console.WriteLine("Error occured: " + e.Message);
            }
        }
    }
}
