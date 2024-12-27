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
    internal class MenuUiAtlasConverter : IMenuState
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
                string outputPath = Path.Combine(FileManager.Instance.TilemapPath, Path.GetFileName(Path.ChangeExtension(inputPath, ".cpp")));
                string jsonOutputPath = Path.GetDirectoryName(inputPath) + Path.DirectorySeparatorChar + Path.GetFileNameWithoutExtension(inputPath) + ".json";
                bool uvsJsonExists = File.Exists(jsonOutputPath);

                UiAtlasConverter.UiAtlasConfig atlasConfig;
                if (uvsJsonExists)
                {
                    // Make a list of new bitmaps that are cloned from sections of the original image. 

                    atlasConfig = JsonConvert.DeserializeObject<Tools.UiAtlasConverter.UiAtlasConfig>(File.ReadAllText(jsonOutputPath));
                    UiAtlasConverter.UiAtlasUvs[] sliceCoordinates = atlasConfig.uvs;

                    var bitmaps = new List<Tools.UiAtlasConverter.UiAtlasBitmap>();

                    foreach (UiAtlasConverter.UiAtlasUvs atlasSlice in sliceCoordinates)
                    {
                        Rectangle rect = new Rectangle(atlasSlice.uvs.x, atlasSlice.uvs.y, atlasSlice.uvs.width, atlasSlice.uvs.height);
                        bitmaps.Add(new Tools.UiAtlasConverter.UiAtlasBitmap() { name = atlasSlice.name, bitmap = bitmap.Clone(rect, bitmap.PixelFormat) } );
                    }

                    UiAtlasConverter converter = new Tools.UiAtlasConverter();
                    converter.Convert(inputPath, outputPath, atlasConfig, bitmaps, PaletteBankIndexOffset);
                }
                else
                {
                    atlasConfig = new UiAtlasConverter.UiAtlasConfig();
                    UiAtlasConverter.UiAtlasUvs[] sliceCoordinates;

                    Console.WriteLine(string.Format("No uvs defined for UI atlas image at {0}. Creating skeleton json file.", inputPath));

                    // Make quick ui atlas uvs cause doing this by hand is tedious af. 
                    if (false)
                    {
                        var sliceCoordinatesList = new List<UiAtlasConverter.UiAtlasUvs>();
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
                                var uvs = new UiAtlasConverter.UiAtlasUvs { name = name, uvs = new Tools.SpriteConverter.UVs { x = column * width, y = row * height, width = width, height = height } };
                                sliceCoordinatesList.Add(uvs);
                            }
                        }

                        sliceCoordinates = sliceCoordinatesList.ToArray();
                    }
                    else
                    {
                        sliceCoordinates = new UiAtlasConverter.UiAtlasUvs[1]
                        {
                            new UiAtlasConverter.UiAtlasUvs { name = "UvName1", uvs = new Tools.SpriteConverter.UVs{ x = 0, y = 0, width = 8, height = 8 } },
                        };
                    }

                    atlasConfig.uvs = sliceCoordinates;

                    string uvJsonData = JsonConvert.SerializeObject(atlasConfig, Formatting.Indented);
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
