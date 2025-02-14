#include "GfxScreenFadeOut.h"
#include "engine/gba/graphics/tiles/GBAPaletteBank.h"
#include "engine/engine/engine.h"
#include "engine/time/Time.h"
#include "engine/graphics/tilemap/TilemapRenderer.h"

constexpr int BgPaletteBufferIndex = 0;

GfxScreenFadeOut::GfxScreenFadeOut(const Colour& destColour, float fadeSpeed)
	: m_invSpeed(1.0f / fadeSpeed)
{
	auto destColourRgb16 = destColour.RGB16();
	m_destColourDecompressed = Colour::DecompressRgb16(destColourRgb16);
}

void GfxScreenFadeOut::CapturePalettes()
{
	m_destPalettes.GetPrimary() = m_originalPalettesCaptured == BgPaletteBufferIndex ? GBA::Gfx::PaletteBank::EditBackgroundPalette() : GBA::Gfx::PaletteBank::EditSpritePalette();

	ColourPalette256 originalPalettes;

	// Take a snapshot of the current palettes
	VramSafeMemCopy(m_originalPalettes.GetPrimary(), *m_destPalettes.GetPrimary(), ARRAY_SIZE(*m_destPalettes.GetPrimary()));

	m_originalPalettes.Flip();
	m_destPalettes.Flip();

	m_originalPalettesCaptured += 1;
	if (m_originalPalettesCaptured >= 2)
	{
		AdvanceState();
	}
}

void GfxScreenFadeOut::FadePalettes()
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

void GfxScreenFadeOut::AdvanceState()
{
	m_currentState = (FadeState)(m_currentState + 1);
}

void GfxScreenFadeOut::Update()
{
	switch (m_currentState)
	{
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
			m_destPaletteResult[i] = Colour::LerpRgb16(decompressedSrc, m_destColourDecompressed, m_t);
		}

		m_t += dt * m_invSpeed;

		if (m_t >= 1)
		{
			m_t = 1;
		}
		break;
	}
	case DisableAndRestoreBgs:
	{
		auto* entityManager = Engine::GetInstance().GetEntityRegistry();
		entityManager->InvokeEach<Component::TilemapRenderer>(
			[](Component::TilemapRenderer& tilemapRenderer)
			{
				tilemapRenderer.SetVisible(false);
			});
		break;
	}
	default:
		break;
	}
}

void GfxScreenFadeOut::LateRender()
{
	switch (m_currentState)
	{
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
	case DisableAndRestoreBgs:
	{
		// Ensure that we're on the bg palette here
		if (m_originalPalettes.GetIndex() != BgPaletteBufferIndex)
		{
			m_originalPalettes.Flip();
			m_destPalettes.Flip();
		}

		VramSafeMemCopy(*m_destPalettes.GetPrimary(), m_originalPalettes.GetPrimary(), ARRAY_SIZE(m_originalPalettes.GetPrimary()));
		AdvanceState();
		break;
	}
	default:
		break;
	}
}

bool GfxScreenFadeOut::IsComplete()
{
	return m_currentState == Complete;
}

void GfxScreenFadeOut::Abort()
{
	m_currentState = Complete;
}
