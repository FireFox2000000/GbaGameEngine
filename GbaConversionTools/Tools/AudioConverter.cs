using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Collections.Generic;

namespace GbaConversionTools.Tools
{
    class AudioConverter
    {
        public void Convert(string inputPath, string outputPath)
        {
            using (BinaryReader reader = new BinaryReader(File.Open(inputPath, FileMode.Open)))
            {
                CppWriter cppWriter = new CppWriter(Path.GetFileName(Path.GetFileNameWithoutExtension(inputPath)), outputPath);

                int sampleRate = 44100; // TODO, currently temp hardcoding. Eventually read from an audio file that has a proper header

                // Write header information
                cppWriter.Write(sampleRate);

                Console.WriteLine("Processing audio samples");

                List<byte> totalBytes = new List<byte>();

                // Copy Signed 8-bit PCM data
                while (reader.BaseStream.Position != reader.BaseStream.Length)
                {
                    totalBytes.Add(reader.ReadByte());
                }

                cppWriter.Write(totalBytes.Count);

                foreach (byte b in totalBytes)
                {
                    cppWriter.Write(b);
                }

                Console.WriteLine("Audio file \"" + outputPath + "\" successfully converted");

                cppWriter.Finalise();
            }
        }
    }
}
