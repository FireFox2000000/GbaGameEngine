#pragma once
#include "engine/graphics/postprocessing/IPostProcessingGfxTask.h"
#include "engine/base/colour/Palette.h"
#include "engine/base/core/stl/DoubleBuffer.h"
#include "engine/base/colour/Colour.h"

class Engine;

class GfxScreenFadeIn : public IPostProcessingGfxTask
{
	enum FadeState
	{
		PaletteCapture,
		FadeRender,
		Complete,

		Count
	};

	int m_originalPalettesCaptured = 0;
	int m_fadeCompleteCount = 0;	// Need to make sure both double-buffers get rendered with t == 1 by the end

	DoubleBuffer<ColourPalette256> m_originalPalettes;
	DoubleBuffer<volatile ColourPalette256*> m_destPalettes;
	ColourPalette256 m_destPaletteResult;	// Pre-calc our lerp results into here and then memcpy into vram during render. Otherwise we get uncomfortably close to VBlank limit. 

	ColourRgb16Decompressed m_startColourDecompressed;
	Rgb16 m_startColour = 0;
	Colour::tColourLerpT m_t = 0;
	tFixedPoint24 m_invSpeed = 1;
	FadeState m_currentState = PaletteCapture;

	void CapturePalettes(Engine* engine);
	void FadePalettes(Engine* engine);
	void AdvanceState();

public:
	GfxScreenFadeIn(const Colour& destColour, float fadeSpeed);

	// Main update loop. Pre-prepare any calcs here.
	void Update(Engine* engine) override;

	// Called after main render
	void LateRender(Engine* engine) override;

	bool IsComplete() override;

	void Abort() override;
};
