#include "Screen.h"
#include "gbatek/DisplayControl.h"

Vector2<int> Screen::GetResolution()
{
	GBATEK::VideoMode mode = GBATEK::ioRegisterDisplayControl->videoMode;

	switch (mode)
	{
	case (GBATEK::VideoMode::Mode5): return { 160, 128 };
	default: return { 240, 160 };
	}
}
