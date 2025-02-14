#include "GfxScreenFadeIn.h"
#include "engine/gba/graphics/tiles/GBAPaletteBank.h"
#include "engine/engine/engine.h"
#include "engine/time/Time.h"
#include "engine/graphics/tilemap/TilemapRenderer.h"

GfxScreenFadeIn::GfxScreenFadeIn(const Colour& destColour, float fadeSpeed)
	: m_invSpeed(1.0f / fadeSpeed)
{
	m_startColour = destColour.RGB16();
	m_startColourDecompressed = Colour::DecompressRgb16(m_startColour);
}

void GfxScreenFadeIn::CapturePalettes()
{
	m_destPalettes.GetPrimary() = m_originalPalettesCaptured == 0 ? GBA::Gfx::PaletteBank::EditBackgroundPalette() : GBA::Gfx::PaletteBank::EditSpritePalette();

	ColourPalette256 originalPalettes;

	// Take a snapshot of the current palettes
	VramSafeMemCopy(m_originalPalettes.GetPrimary(), *m_destPalettes.GetPrimary(), ARRAY_SIZE(*m_destPalettes.GetPrimary()));

	// Set the palettes to the start colour
	{
		auto* palette = m_destPalettes.GetPrimary();
		VramSafeMemSet(*palette, m_startColour, ARRAY_SIZE(*palette));
	}

	m_originalPalettes.Flip();
	m_destPalettes.Flip();

	m_originalPalettesCaptured += 1;
	if (m_originalPalettesCaptured >= 2)
	{
		AdvanceState();
	}
}

void GfxScreenFadeIn::FadePalettes()
{
	// Now we can actually apply the lerp
	{
		VramSafeMemCopy(*m_destPalettes.GetPrimary(), m_destPaletteResult, ARRAY_SIZE(m_destPaletteResult));
	}

	// Interpolate the background and sprite palettes on different frames to reduce workload. 
	m_originalPalettes.Flip();
	m_destPalettes.Flip();

	if (m_t >= 1)
	{
		m_fadeCompleteCount += 1;

		if (m_fadeCompleteCount >= 2)
		{
			AdvanceState();
		}
	}
}

void GfxScreenFadeIn::AdvanceState()
{
	m_currentState = (FadeState)(m_currentState + 1);
}

void GfxScreenFadeIn::Update()
{
	switch (m_currentState)
	{
	case EnsureBackgroundsHidden:
	{
		auto* entityManager = Engine::GetInstance().GetEntityRegistry();
		entityManager->InvokeEach<TilemapRenderer>(
			[](TilemapRenderer& tilemapRenderer)
			{
				tilemapRenderer.SetVisible(false);
			});
		break;
	}
	case EnsureBackgroundsVisible:
	{
		auto* entityManager = Engine::GetInstance().GetEntityRegistry();
		entityManager->InvokeEach<TilemapRenderer>(
			[](TilemapRenderer& tilemapRenderer)
			{
				tilemapRenderer.SetVisible(true);
			});
		AdvanceState();
		// Fall through to fade render
	}
	case FadeRender:
	{
		// Calculate the new palette

		const Time* time = Engine::GetInstance().GetComponent<Time>();
		FPi24 dt = time->GetDt();

		auto& srcPalette = m_originalPalettes.GetPrimary();

		// Perform the heavy calcs here before rendering
		for (u32 i = 0; i < ARRAY_SIZE(srcPalette); ++i)
		{
			auto decompressedSrc = Colour::DecompressRgb16(srcPalette[i]);
			m_destPaletteResult[i] = Colour::LerpRgb16(m_startColourDecompressed, decompressedSrc, m_t);
		}

		m_t += dt * m_invSpeed;

		if (m_t >= 1)
		{
			m_t = 1;
		}
		break;
	}
	default:
		break;
	}
}

void GfxScreenFadeIn::LateRender()
{
	switch (m_currentState)
	{
	case EnsureBackgroundsHidden:
	{
		AdvanceState();
		break;
	}
	case PaletteCapture:
	{
		CapturePalettes();
		break;
	}
	case FadeRender:
	{
		FadePalettes();
		break;
	}
	default:
		break;
	}
}

bool GfxScreenFadeIn::IsComplete()
{
	return m_currentState == Complete;
}

void GfxScreenFadeIn::Abort()
{
	m_currentState = Complete;
}