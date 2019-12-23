using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GbaConversionTools
{
    static class MathX
    {
        public static bool IsPowerOf2(uint v)
        {
            return v != 0 && !((v & (v - 1)) != 0);
        }

        public static int IndexOfHighestSetBit(uint v)
        {
            int msb = 0;

            if (v > 0)
            {
                while (v != 0)
                {
                    v >>= 1;
                    msb++;
                }

                --msb;
            }

            return msb;
        }

        public static uint NextPowerOf2(uint v)
        {
            v--;
            v |= v >> 1;
            v |= v >> 2;
            v |= v >> 4;
            v |= v >> 8;
            v |= v >> 16;
            v++;

            return v;
        }
    }
}
