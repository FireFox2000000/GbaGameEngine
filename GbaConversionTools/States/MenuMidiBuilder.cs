using System.IO;

namespace GbaConversionTools.States
{
    internal class MenuMidiBuilder : IMenuState
    {
        public void Enter()
        {
            DmgMidis.MidiFallOfFall.MakeMidi("DmgMidiFallOfFall.dmg", Path.Combine(FileManager.Instance.AudioPath, "DmgMidiFallOfFall.cpp"));
        }
    }
}
