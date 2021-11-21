using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using NAudio.Wave;
using System.Runtime;

namespace GbaConversionTools.Tools
{
    class AudioConverter
    {
        public void Convert(string inputPath, string outputPath)
        {
            string ext = Path.GetExtension(inputPath);
            if (ext == ".mp3")
            {
                using (var reader = new Mp3FileReader(inputPath))
                {
                    WriteFromWaveStream(inputPath, outputPath, reader);
                }
            }
            else if (ext == ".wav")
            {
                using (var reader = new WaveFileReader(inputPath))
                {
                    WriteFromWaveStream(inputPath, outputPath, reader);
                }
            }
            else
            {
                throw new Exception("File format not supported.");
            }
        }

        void WriteFromWaveStream(string inputPath, string outputPath, WaveStream reader)
        {
            var waveFormat = reader.WaveFormat;
            int sampleRate = waveFormat.SampleRate;

            using (WaveStream pcmStream = WaveFormatConversionStream.CreatePcmStream(reader))
            {
                WaveFormat newFormat = new WaveFormat(sampleRate, 8, 1);
                using (WaveStream eightBitPcmStream = new WaveFormatConversionStream(newFormat, pcmStream))
                {
                    CppWriter cppWriter = new CppWriter(Path.GetFileName(Path.GetFileNameWithoutExtension(inputPath)), outputPath);
                    cppWriter.Write(sampleRate);

                    List<byte> totalBytes = new List<byte>();
                    int b = eightBitPcmStream.ReadByte();
                    while (b != -1)
                    {
                        totalBytes.Add((byte)(b + 128));    // Make it signed 8-bit PCM, as this comes through as unsigned
                        b = eightBitPcmStream.ReadByte();
                    }

                    cppWriter.Write(totalBytes.Count);

                    foreach (byte data in totalBytes)
                    {
                        cppWriter.Write(data);
                    }

                    cppWriter.Finalise();
                    Console.WriteLine("Audio file \"" + outputPath + "\" successfully converted");
                }
            }      
        }
    }
}
