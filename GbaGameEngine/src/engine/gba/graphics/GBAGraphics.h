#pragma once
namespace GBA
{
	enum GraphicsDisplayParams
	{
		IO_Mode0_Tile = 0x0,
		IO_Mode1_Tile = 0x1,
		IO_Mode2_Tile = 0x2,

		IO_Mode3_Bitmap = 0x3,
		IO_Mode4_Bitmap = 0x4,
		IO_Mode5_Bitmap = 0x5,

		IO_Background0 = 0x100,
		IO_Background1 = 0x200,
		IO_Background2 = 0x400,
		IO_Background3 = 0x800,

		IO_OAM = 0x1000,
	};

	class Graphics
	{
	public:
		static void SetDisplayParameters(int params);
	};
}
