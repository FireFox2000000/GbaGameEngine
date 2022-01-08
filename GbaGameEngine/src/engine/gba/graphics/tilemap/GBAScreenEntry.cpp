#include "GBAScreenEntry.h"

void GBA::Gfx::ScreenEntry::SetTileIndex(int index)
{
    m_tileIndex = index;
}

void GBA::Gfx::ScreenEntry::SetHFlipFlag()
{
    m_hFlip = true;
}

void GBA::Gfx::ScreenEntry::SetVFlipFlag()
{
    m_vFlip = true;
}

void GBA::Gfx::ScreenEntry::SetPalIndex(int index)
{
    m_palIndex = index;
}
