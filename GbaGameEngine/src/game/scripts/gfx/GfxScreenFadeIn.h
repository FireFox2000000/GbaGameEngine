#pragma once
#include "engine/graphics/postprocessing/IPostProcessingGfxTask.h"
#include "engine/base/colour/Palette.h"
#include "engine/base/core/stl/DoubleBuffer.h"
#include "engine/base/colour/Colour.h"

class Engine;

class GfxScreenFadeIn : public IPostProcessingGfxTask
{
private:
	constexpr static GBA::ColourRGB16 Black{ 0, 0, 0 };

public:
	enum FadeState
	{
		EnsureBackgroundsHidden,
		PaletteCapture,
		EnsureBackgroundsVisible,
		FadeRender,
		Complete,

		Count
	};

	int m_originalPalettesCaptured = 0;
	int m_fadeCompleteCount = 0;	// Need to make sure both double-buffers get rendered with t == 1 by the end

	DoubleBuffer<GBA::ColourPalette> m_originalPalettes;
	DoubleBuffer<GBA::ColourPalette*> m_destPalettes;
	GBA::ColourPalette m_destPaletteResult;	// Pre-calc our lerp results into here and then memcpy into vram during render. Otherwise we get uncomfortably close to VBlank limit. 

	ColourRgb16Decompressed m_startColourDecompressed;
	
	GBA::ColourRGB16 m_startColour = Black;
	Colour::tColourLerpT m_t = 0;
	tFixedPoint24 m_invSpeed = 1;
	FadeState m_currentState = PaletteCapture;

	void CapturePalettes();
	void FadePalettes();
	void AdvanceState();

public:
	GfxScreenFadeIn(const Colour& destColour, float fadeSpeed);

	// Main update loop. Pre-prepare any calcs here.
	void Update() override;

	// Called after main render
	void LateRender() override;

	bool IsComplete() override;

	void Abort() override;
};
