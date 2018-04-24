#include "engine\gba\registers\display\GBADisplayStatus.h"
#include "engine\gba\registers\input\GBAInput.h"
#include "engine\scene\SceneManager.h"

#include "game\Scene0.h"

#define VBLANK_SCNLNE_START 160

static void WaitForVSync();

int main()
{
	Scene0 scene0;
	SceneManager sceneManager(&scene0);

	// Test Initialisation		
	GBA::Input::Update();

	// Update loop
	while (true)
	{
		// General update
		GBA::Input::Update();

		sceneManager.UpdateScene();

		// Main update
		WaitForVSync();

		// Render
		sceneManager.RenderScene();
	}

	return 0;
}

static void WaitForVSync()
{
	while (GBA::DisplayStatus::VCount() >= VBLANK_SCNLNE_START);   // wait till VDraw
	while (GBA::DisplayStatus::VCount() < VBLANK_SCNLNE_START);    // wait till VBlank
}
