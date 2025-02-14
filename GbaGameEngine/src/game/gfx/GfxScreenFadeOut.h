#pragma once
#include "engine/graphics/postprocessing/IPostProcessingGfxTask.h"
#include "engine/base/colour/Palette.h"
#include "engine/base/core/stl/DoubleBuffer.h"
#include "engine/base/colour/Colour.h"

class Engine;

class GfxScreenFadeOut : public IPostProcessingGfxTask
{
	enum FadeState
	{
		PaletteCapture,
		FadeRender,
		DisableAndRestoreBgs,
		Complete,

		Count
	};

	int m_originalPalettesCaptured = 0;
	int m_fadeCompleteCount = 0;	// Need to make sure both double-buffers get rendered with t == 1 by the end

	DoubleBuffer<GBATEK::ColourPalette> m_originalPalettes;
	DoubleBuffer<GBATEK::ColourPalette*> m_destPalettes;
	GBATEK::ColourPalette m_destPaletteResult;	// Pre-calc our lerp results into here and then memcpy into vram during render. Otherwise we get uncomfortably close to VBlank limit. 

	ColourRgb16Decompressed m_destColourDecompressed;
	Colour::tColourLerpT m_t = 0;
	FPi24 m_invSpeed = 1;
	FadeState m_currentState = PaletteCapture;

	void CapturePalettes();
	void FadePalettes();
	void AdvanceState();

public:
	GfxScreenFadeOut(const Colour& destColour, float fadeSpeed);

	// Main update loop. Pre-prepare any calcs here.
	void Update() override;

	// Called after main render
	void LateRender() override;

	bool IsComplete() override;

	void Abort() override;
};
