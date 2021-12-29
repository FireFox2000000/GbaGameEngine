#include "GfxScreenFadeIn.h"
#include "engine/gba/graphics/tiles/GBAPaletteBank.h"
#include "engine/engine/engine.h"
#include "engine/time/Time.h"

GfxScreenFadeIn::GfxScreenFadeIn(const Colour& destColour, float fadeSpeed)
	: m_invSpeed(1.0f / fadeSpeed)
{
	m_startColour = Colour::RGB16(destColour.r, destColour.g, destColour.b);
}

void GfxScreenFadeIn::CapturePalettes(Engine* engine)
{
	m_destPalettes.GetPrimary() = GBA::Gfx::PaletteBank::EditBackgroundPalette();
	m_destPalettes.GetSecondary() = GBA::Gfx::PaletteBank::EditSpritePalette();

	VramSafeMemCopy((void*)m_destPalettes.GetPrimary(), &m_originalPalettes.GetPrimary(), m_originalPalettes.GetPrimary().Count() * sizeof(Rgb16));
	VramSafeMemCopy((void*)m_destPalettes.GetSecondary(), &m_originalPalettes.GetSecondary(), m_originalPalettes.GetSecondary().Count() * sizeof(Rgb16));

	{
		auto& palette = m_destPalettes.GetPrimary();
		VramSafeMemSet((void*)&palette[0], m_startColour, palette->Count());
	}

	{
		auto& palette = m_destPalettes.GetSecondary();
		VramSafeMemSet((void*)&palette[0], m_startColour, palette->Count());
	}

	m_originalPalettesCaptured = true;
}

void GfxScreenFadeIn::FadePalettes(Engine* engine)
{
	const Time* time = engine->GetComponent<Time>();
	tFixedPoint24 dt = time->GetDt();

	// Now we can actually apply the lerp
	{
		ColourPalette256& srcPalette = m_originalPalettes.GetPrimary();
		volatile ColourPalette256& destPalette = *m_destPalettes.GetPrimary();

		for (u32 i = 0; i < srcPalette.Count(); ++i)
		{
			destPalette[i] = Colour::LerpRgb16(m_startColour, srcPalette[i], m_t);
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

void GfxScreenFadeIn::Update(Engine* engine)
{
}

void GfxScreenFadeIn::LateRender(Engine* engine)
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

bool GfxScreenFadeIn::IsComplete()
{
	return m_fadeCompleteCount > 1;
}

void GfxScreenFadeIn::Abort()
{
	m_fadeCompleteCount = 5;
}