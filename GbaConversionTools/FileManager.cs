using System.IO;

namespace GbaConversionTools
{
    internal class FileManager
    {
        string m_outputDir;


        public FileManager(string outputDir)
        {
            m_outputDir = outputDir;
        }

        string GetOutputFolder(string key)
        {
            string folder = Path.Combine(m_outputDir, key);

            if (!Directory.Exists(folder))
            {
                Directory.CreateDirectory(folder);
            }

            return folder;
        }

        public string AudioPath { get { return GetOutputFolder("audio"); } }
        public string SpritePath { get { return GetOutputFolder("sprites"); } }
        public string TilemapPath { get { return GetOutputFolder("tilemaps"); } }

        public static FileManager Instance = null;
    }
}
