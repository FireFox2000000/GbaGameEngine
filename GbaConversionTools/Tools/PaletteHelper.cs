using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;

namespace GbaConversionTools.Tools
{
    class PaletteHelper
    {
        public static readonly int PALETTE_LENGTH_4BBP = 16;
        public static readonly int MAX_PALETTE_LENGTH = 256;
        public static readonly int MAX_PALETTE_INDEX = 16;

        static bool IsAlpha(Color colour)
        {
            return colour.A <= 0;
        }

        public static Color[] GeneratePaletteFromImage(Bitmap bitmap, int xStart, int yStart, int width, int height)
        {
            List<Color> palette = new List<Color>();
            Color transparencyColour = Color.FromArgb(0);
            palette.Add(transparencyColour);     // Always make transparent the first index

            for (int y = yStart; y < height; ++y)
            {
                for (int x = xStart; x < width; ++x)
                {
                    Color color = bitmap.GetPixel(x, y);

                    if (color.A != 0 && color.A != 255)
                    {
                        Console.WriteLine(string.Format("Warning: Found pixel with invalid alpha value ({2}) at position ({0}, {1}). Transparency is not allowed.", x, y, color.A));
                    }

                    bool foundInPalette = false;

                    foreach (Color palColor in palette)
                    {
                        // Don't need to compare alpha
                        if (color == palColor || (IsAlpha(palColor) && IsAlpha(color)))
                        {
                            foundInPalette = true;
                            break;
                        }
                    }

                    if (!foundInPalette)
                        palette.Add(color);
                }
            }

            return palette.ToArray();
        }

        public static int ColorToPaletteIndex(Color[] palette, Color color)
        {
            for (int i = 0; i < palette.Length; ++i)
            {
                if ((IsAlpha(color) && IsAlpha(palette[i])) || color == palette[i])
                    return i;
            }

            throw new Exception("Colour not found in palette");
        }

        public static byte ScaleToRgb16(byte colorChannelValue)
        {
            return (byte)(Math.Round(colorChannelValue / 255.0f * 31.0f));
        }

        public static UInt16 ToRgb16(Color colour)
        {
            return (UInt16)(ScaleToRgb16(colour.R) + (ScaleToRgb16(colour.G) << 5) + (ScaleToRgb16(colour.B) << 10));
        }
    }
}
