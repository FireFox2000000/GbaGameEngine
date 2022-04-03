using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GbaConversionTools
{
    static class Defines
    {
        public const string TAB_CHAR = "\t";
        public const string STR_U8 = "unsigned char";
        public const string STR_U16 = "unsigned short";
        public const string STR_U32 = "unsigned long";

        // const gets put into rom automatically, was accidently forcing this into EWRAM due to confusing with multicart builds. 
        public const string STR_DEFINE_MACRO_EWRAM_DATA = "";//"#define EWRAM_DATA __attribute__((section(\".ewram\")))";
        public const string STR_EWRAM_DATA = "";//"EWRAM_DATA";

        public const string STR_SPRITEMAP_NAMESPC_PREFIX = "__binary_spritesheet_";       // Maps to SpriteLibrary.cpp SPRITEMAP_NAMESPC_PREFIX
        public const string STR_TILEMAP_NAMESPC_PREFIX = "__binary_background_";

        public const string DATA_PATH = "../../../GbaGameEngine/src/game/data/";
        public static readonly string DATA_PATH_AUDIO = System.IO.Path.Combine(DATA_PATH, "audio");
        public static readonly string DATA_PATH_SPRITE = System.IO.Path.Combine(DATA_PATH, "sprites");
        public static readonly string DATA_PATH_TILEMAP = System.IO.Path.Combine(DATA_PATH, "tilemaps");
    }
}
