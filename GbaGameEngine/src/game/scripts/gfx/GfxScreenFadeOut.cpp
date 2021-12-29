#include "GfxScreenFadeOut.h"
#include "engine/gba/graphics/tiles/GBAPaletteBank.h"
#include "engine/engine/engine.h"
#include "engine/time/Time.h"

GfxScreenFadeOut::GfxScreenFadeOut(const Colour& destColour, float fadeSpeed)
	: m_invSpeed(1.0f / fadeSpeed)
{
	m_destColour = Colour::RGB16(destColour.r, destColour.g, destColour.b);
}

void GfxScreenFadeOut::CapturePalettes(Engine* engine)
{
	m_destPalettes.GetPrimary() = GBA::Gfx::PaletteBank::EditBackgroundPalette();
	m_destPalettes.GetSecondary() = GBA::Gfx::PaletteBank::EditSpritePalette();

	for (u32 i = 0; i < m_destPalettes.GetPrimary()->Count(); ++i)
	{
		m_originalPalettes.GetPrimary()[i] = (*m_destPalettes.GetPrimary())[i];
	}

	for (u32 i = 0; i < m_destPalettes.GetSecondary()->Count(); ++i)
	{
		m_originalPalettes.GetSecondary()[i] = (*m_destPalettes.GetSecondary())[i];
	}

	m_originalPalettesCaptured = true;
}

void GfxScreenFadeOut::FadePalettes(Engine* engine)
{
	const Time* time = engine->GetComponent<Time>();
	tFixedPoint24 dt = time->GetDt();

	// Now we can actually apply the lerp
	{
		ColourPalette256& srcPalette = m_originalPalettes.GetPrimary();
		volatile ColourPalette256& destPalette = *m_destPalettes.GetPrimary();

		for (u32 i = 0; i < srcPalette.Count(); ++i)
		{
			destPalette[i] = Colour::LerpRgb16(srcPalette[i], m_destColour, m_t);
		}
	}

	// Interpolate the background and sprite palettes on different frames to reduce workload. 
	m_originalPalettes.Flip();
	m_destPalettes.Flip();

	if (m_t >= 1)
	{
		m_t = 1;
		m_fadeCompleteCount += 1;
	}
	else
	{
		m_t += dt * m_invSpeed;

		if (m_t >= 1)
		{
			m_t = 1;
		}
	}
}

void GfxScreenFadeOut::Update(Engine* engine)
{
}

void GfxScreenFadeOut::LateRender(Engine* engine)
{
	if (IsComplete()) return;

	if (!m_originalPalettesCaptured)
	{
		CapturePalettes(engine);
	}
	else
	{
		FadePalettes(engine);
	}
}

bool GfxScreenFadeOut::IsComplete()
{
	return m_fadeCompleteCount > 1;
}

void GfxScreenFadeOut::Abort()
{
	m_fadeCompleteCount = 5;
}
