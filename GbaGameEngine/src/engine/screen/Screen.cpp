#include "Screen.h"
#include "engine\gba\registers\display\GBADisplayControl.h"

Vector2 Screen::GetResolution()
{
	return GBA::DisplayControl::GetScreenResolution();
}
