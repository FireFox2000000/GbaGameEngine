using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GbaConversionTools
{
    static class Compression
    {
        public enum CompressionType
        {
            // Maps to GBATek to make things a little easier - https://www.akkit.org/info/gbatek.htm#biosdecompressionfunctions
            // Maximum of 4 bits, or 16 values
            None = 0,
            LZ77 = 1,
            Huffman = 2,
            RunLength = 3,
            DiffFiltered = 8,
            BitPacked = 9,
        }

        public static readonly int COMPRESSED_TYPE_BIT_INDEX = 4;

        public static void BitPack(List<int> src, uint destBpp, out List<UInt32> dest)
        {
            // Should be 1, 2, 4 or 8
            if (!MathX.IsPowerOf2(destBpp) || destBpp > 8)
            {
                throw new System.Exception(string.Format("Dest bits per pixel ({0}) not compatible", destBpp));
            }

            const int BITS_PER_BYTE = 8;
            int valuesPerByte = BITS_PER_BYTE / (int)destBpp;
            int valuesPerDest = sizeof(UInt32) * valuesPerByte;

            dest = new List<UInt32>();

            for (int i = 0; i < src.Count; i += valuesPerDest)
            {
                UInt32 hexNum = 0;
                for (int j = i + valuesPerDest - 1; j >= i; --j)
                {
                    int index = j < src.Count ? src[j] : 0;

                    if (index >= (1 << (int)destBpp))
                    {
                        throw new Exception(string.Format("Found value ({0}) that is not compatible with desired bits per pixel compression ({1}).", index, destBpp));
                    }

                    hexNum += (UInt32)index;

                    if (j > i)
                        hexNum <<= (int)destBpp;
                }

                dest.Add(hexNum);
            }
        }
    }
}
