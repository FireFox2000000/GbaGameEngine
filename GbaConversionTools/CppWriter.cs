using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;
using System.Runtime.InteropServices;

namespace GbaConversionTools
{
    class CppWriter
    {
        const string TAB_CHAR = Defines.TAB_CHAR;

        List<byte> m_binaryData = new List<byte>();
        string m_filePath;
        string m_namespaceName;
        int m_currentIndentationLevel = 0;

        const int MAX_CHARS_PER_LINE = 10;
        int m_currentCharsOnLine = 0;

        public CppWriter(string namespaceName, string filePath)
        {
            m_filePath = filePath;
            m_namespaceName = namespaceName;      
        }

        byte[] GetBytes(Object data)
        { 
            var size = Marshal.SizeOf(data);
            // Both managed and unmanaged buffers required.
            var bytes = new byte[size];
            var ptr = Marshal.AllocHGlobal(size);
            // Copy object byte-to-byte to unmanaged memory.
            Marshal.StructureToPtr(data, ptr, false);
            // Copy data from unmanaged memory to managed buffer.
            Marshal.Copy(ptr, bytes, 0, size);
            // Release unmanaged memory.
            Marshal.FreeHGlobal(ptr);

            return bytes;
            /*
            BinaryFormatter bf = new BinaryFormatter();
            using (MemoryStream ms = new MemoryStream())
            {
                bf.Serialize(ms, data);
                return ms.ToArray();
            }*/
        }

        public void Write(Object data)
        {
            byte[] bytes = GetBytes(data);

            // Will need to pad this based on the size of the data so that memory is aligned correctly when we read it back out
            int size = Marshal.SizeOf(data);
            int currentSize = m_binaryData.Count;
            int remainder = currentSize % size;
            int paddingBytes = currentSize > 0 && remainder > 0 ? size - remainder : 0;

            for (int i = 0; i < paddingBytes; ++i)
            {
                m_binaryData.Add(0);
            }

            m_binaryData.AddRange(bytes);
        }

        public void Finalise()
        {
            WriteCppFile();

            string headerFilePath = Path.ChangeExtension(m_filePath, ".h");
            bool generateHeader = !File.Exists(headerFilePath);
            if (generateHeader)
            {
                WriteHeaderFile(headerFilePath);
            }
        }

        void AppendIndentation(StringBuilder sb, int level)
        {
            for (int i = 0; i < level; ++i)
            {
                sb.Append(TAB_CHAR);
            }
        }

        void WriteCppFile()
        {
            StringBuilder sb = new StringBuilder();

            string headerInclude = string.Format("#include \"{0}.h\"", Path.GetFileNameWithoutExtension(m_filePath));
            sb.AppendLine(headerInclude);
            sb.AppendLine();

            // Open up the array
            sb.AppendFormat("const unsigned long {0}::data[] = {{\n", m_namespaceName);

            ++m_currentIndentationLevel;
            AppendIndentation(sb, m_currentIndentationLevel);

            //m_binaryData.InsertRange(0, GetBytes(m_binaryData.Count));

            // Write our array data
            UInt32 currentNum = 0;
            int hexCount = 0;
            List<UInt32> hexNums = new List<UInt32>();
            foreach (byte c in m_binaryData)
            {
                /*
                currentNum <<= 8;
                currentNum |= c;
                ++hexCount;
                */
                UInt32 b32 = c;
                b32 <<= 8 * hexCount++;
                currentNum |= b32;

                if (hexCount >= sizeof(UInt32))
                {
                    hexNums.Add(currentNum);
                    currentNum = 0;
                    hexCount = 0;
                }
            }

            if (hexCount > 0)
            {
                hexNums.Add(currentNum);
            }

            foreach (var c in hexNums)
            {
                sb.AppendFormat("0x{0:X8}, ", c);
                ++m_currentCharsOnLine;

                if (m_currentCharsOnLine >= MAX_CHARS_PER_LINE)
                {
                    sb.AppendLine();
                    AppendIndentation(sb, m_currentIndentationLevel);

                    m_currentCharsOnLine = 0;
                }
            }

            sb.AppendLine();
            --m_currentIndentationLevel;
            AppendIndentation(sb, m_currentIndentationLevel);
            sb.AppendLine("};");   // Close the data array

            File.WriteAllText(m_filePath, sb.ToString());
        }

        void WriteHeaderFile(string outputPath)
        {
            StringBuilder sb = new StringBuilder();
            sb.AppendLine("#pragma once");
            sb.AppendLine();

            sb.AppendLine(string.Format("class {0}", m_namespaceName));
            sb.AppendLine("{");
            sb.AppendLine("public:");
            AppendIndentation(sb, 1);
            sb.AppendLine("static const unsigned long data[];");
            sb.AppendLine("};");

            File.WriteAllText(outputPath, sb.ToString());
        }
    }
}
