using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GbaConversionTools.Tools;

namespace GbaConversionTools.States
{
    internal class MenuMidiBuilder : IMenuState
    {
        public void Enter()
        {
            DmgMidis.MidiFallOfFall.MakeMidi("DmgMidiFallOfFall.dmg", "DmgMidiFallOfFall.cpp");
        }
    }
}
