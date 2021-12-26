#include "GBAScreenEntry.h"

void GBA::Gfx::ScreenEntry::SetTileIndex(int index)
{
    m_data |= (u16)(index & 0b1111111111);
}

void GBA::Gfx::ScreenEntry::SetHFlipFlag()
{
    m_data |= 1 << 10;
}

void GBA::Gfx::ScreenEntry::SetVFlipFlag()
{
    m_data |= 1 << 11;
}

void GBA::Gfx::ScreenEntry::SetPalIndex(int index)
{
    m_data |= (u16)((index & 0b1111) << 0xc);
}
