#include "Screen.h"
#include "engine\gba\graphics\displayregisters\GBADisplayControl.h"

Vector2 Screen::GetResolution()
{
	return GBA::DisplayControl::GetScreenResolution();
}
