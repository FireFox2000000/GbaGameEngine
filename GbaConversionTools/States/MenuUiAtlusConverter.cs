using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Drawing;
using System.Drawing.Imaging;
using System.Numerics;
using Newtonsoft.Json;
using GbaConversionTools.Tools;

namespace GbaConversionTools.States
{
    internal class MenuUiAtlusConverter : IMenuState
    {
        public void Enter()
        {
            WalkConverterOptions();
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

                UiAtlusConverter.UiAtlusConfig atlusConfig;
                if (uvsJsonExists)
                {
                    // Make a list of new bitmaps that are cloned from sections of the original image. 

                    atlusConfig = JsonConvert.DeserializeObject<Tools.UiAtlusConverter.UiAtlusConfig>(File.ReadAllText(jsonOutputPath));
                    UiAtlusConverter.UiAtlusUvs[] sliceCoordinates = atlusConfig.uvs;

                    var bitmaps = new List<Tools.UiAtlusConverter.UiAtlusBitmap>();

                    foreach (UiAtlusConverter.UiAtlusUvs atlusSlice in sliceCoordinates)
                    {
                        Rectangle rect = new Rectangle(atlusSlice.uvs.x, atlusSlice.uvs.y, atlusSlice.uvs.width, atlusSlice.uvs.height);
                        bitmaps.Add(new Tools.UiAtlusConverter.UiAtlusBitmap() { name = atlusSlice.name, bitmap = bitmap.Clone(rect, bitmap.PixelFormat) } );
                    }

                    UiAtlusConverter converter = new Tools.UiAtlusConverter();
                    converter.Convert(inputPath, outputPath, atlusConfig, bitmaps, PaletteBankIndexOffset);
                }
                else
                {
                    atlusConfig = new UiAtlusConverter.UiAtlusConfig();
                    UiAtlusConverter.UiAtlusUvs[] sliceCoordinates;

                    Console.WriteLine(string.Format("No uvs defined for UI atlus image at {0}. Creating skeleton json file.", inputPath));

                    // Make quick ui atlus uvs cause doing this by hand is tedious af. 
                    if (false)
                    {
                        var sliceCoordinatesList = new List<UiAtlusConverter.UiAtlusUvs>();
                        int rows = 6;
                        int columns = 16;
                        int width = 8;
                        int height = 8;

                        int asciiNumber = 33;
                        for (int row = 0; row < rows; ++row)
                        {
                            for (int column = 0; column < columns; ++column)
                            {
                                string name = string.Format("Ascii_{0}", asciiNumber++);
                                var uvs = new UiAtlusConverter.UiAtlusUvs { name = name, uvs = new Tools.SpriteConverter.UVs { x = column * width, y = row * height, width = width, height = height } };
                                sliceCoordinatesList.Add(uvs);
                            }
                        }

                        sliceCoordinates = sliceCoordinatesList.ToArray();
                    }
                    else
                    {
                        sliceCoordinates = new UiAtlusConverter.UiAtlusUvs[1]
                        {
                            new UiAtlusConverter.UiAtlusUvs { name = "UvName1", uvs = new Tools.SpriteConverter.UVs{ x = 0, y = 0, width = 8, height = 8 } },
                        };
                    }

                    atlusConfig.uvs = sliceCoordinates;

                    string uvJsonData = JsonConvert.SerializeObject(atlusConfig, Formatting.Indented);
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
