#pragma once
#include "engine/graphics/postprocessing/IPostProcessingGfxTask.h"
#include "engine/base/colour/Palette.h"
#include "engine/base/core/stl/DoubleBuffer.h"
#include "engine/base/colour/Colour.h"

class Engine;

class GfxScreenFadeIn : public IPostProcessingGfxTask
{
	bool m_originalPalettesCaptured = false;
	int m_fadeCompleteCount = 0;	// Need to make sure both double-buffers get rendered with t == 1 by the end

	DoubleBuffer<ColourPalette256> m_originalPalettes;
	DoubleBuffer<volatile ColourPalette256*> m_destPalettes;

	Rgb16 m_startColour = 0;
	Colour::tColourLerpT m_t = 0;
	tFixedPoint24 m_invSpeed = 1;

	void CapturePalettes(Engine* engine);
	void FadePalettes(Engine* engine);

public:
	GfxScreenFadeIn(const Colour& destColour, float fadeSpeed);

	// Main update loop. Pre-prepare any calcs here.
	void Update(Engine* engine) override;

	// Called after main render
	void LateRender(Engine* engine) override;

	bool IsComplete() override;

	void Abort() override;
};
