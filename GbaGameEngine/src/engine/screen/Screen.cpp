#include "Screen.h"
#include "gbatek/DisplayControl.h"

Vector2<int> Screen::GetResolution()
{
	GBA::VideoMode mode = GBA::ioRegisterDisplayControl->videoMode;

	switch (mode)
	{
	case (GBA::VideoMode::Mode5): return { 160, 128 };
	default: return { 240, 160 };
	}
}
