#include "GBAInput.h"
#include "engine/gba/registers/RegisterMap.h"

vu32& GBA::Input::s_REG_KEYINPUT = (*(vu32*)(REG_KEYINPUT));

u32 GBA::Input::m_previousInput = 0;
u32 GBA::Input::m_currentInput = 0;

void GBA::Input::Update()
{
	GBA::Input::m_previousInput = GBA::Input::m_currentInput;
	GBA::Input::m_currentInput = s_REG_KEYINPUT;
}
