#include "GBAInput.h"
#define KEYS (*(u32*)0x04000130)

u32 GBA::Input::m_previousInput = 0;
u32 GBA::Input::m_currentInput = 0;

void GBA::Input::Update()
{
	GBA::Input::m_previousInput = GBA::Input::m_currentInput;
	GBA::Input::m_currentInput = KEYS;
}
