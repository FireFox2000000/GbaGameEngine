using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Imaging;

namespace GbaConversionTools
{
    class Program
    {
        const int c_hexValueWrapCount = 10;

        static void AnyKeyToContinue()
        {
            Console.WriteLine("Press any key to continue");
            Console.ReadKey();
        }

        static int ColorToPaletteIndex(ColorPalette palette, Color color)
        {
            for(int i = 0; i < palette.Entries.Length; ++i)
            {
                if (color == palette.Entries[i])
                    return i;
            }

            throw new Exception("Colour not found in palette");
        }

        static byte ScaleToRgb16(byte colorChannelValue)
        {
            return (byte)(Math.Round(colorChannelValue / 255.0f * 31.0f));
        }

        static void Main(string[] args)
        {
            // Todo, remove hardcoded filenames
            string filename = "Shantae.bmp";
            string outputfile = "ShantaeBitmap.h";
            
            try
            {
                Bitmap bitmap = new Bitmap(filename);
                if (bitmap.PixelFormat != System.Drawing.Imaging.PixelFormat.Format4bppIndexed)
                {
                    throw new Exception("Tool currently only supports 4bpp formats");
                }

                StringBuilder sb = new StringBuilder();
                Size size = bitmap.Size;
                ColorPalette palette = bitmap.Palette;

                int hexCount = 0;

                sb.AppendFormat("u8 width = {0}, height = {1}; \n\n", size.Width, size.Height);
                sb.Append("const u16 spriteData[] = {\n\t");

                // Write all palette indicies
                for (int y = 0; y < size.Height; ++y)
                {
                    for (int x = 0; x < size.Width; x += 4)
                    {
                        UInt16 hexNum = 0;
                        for (int i = 0; i < 4; ++i)
                        {
                            if (i > 0)
                                hexNum <<= 4;

                            Color color = bitmap.GetPixel(x + i, y);
                            int index = ColorToPaletteIndex(palette, color);
                            hexNum += (UInt16)index;
                        }
                        
                        sb.AppendFormat("0x{0:X4}, ", hexNum);
                        if ((hexCount + 1) % c_hexValueWrapCount == 0)
                        {
                            sb.Append("\n\t");
                        }
                        ++hexCount;
                    }
                }
                sb.Append("\n};\n\n");

                // Write palette
                sb.Append("const u16 palette[] = {\n\t");
                for (int i = 0; i < palette.Entries.Length; ++i)
                {
                    Color color = palette.Entries[i];
                    UInt16 rbgColor = (UInt16)(ScaleToRgb16(color.R) + (ScaleToRgb16(color.G) << 5) + (ScaleToRgb16(color.B) << 10));

                    sb.AppendFormat("0x{0:X4}, ", rbgColor);
                    if ((i + 1) % c_hexValueWrapCount == 0)
                    {
                        sb.Append("\n\t");
                    }
                }
                sb.Append("\n};\n");

                Console.WriteLine(sb);
                System.IO.File.WriteAllText(outputfile, sb.ToString());
            }
            catch (Exception e)
            {
                Console.WriteLine("Error occured: " + e.Message);
            }

            AnyKeyToContinue();
        }
    }
}
