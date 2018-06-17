#include "engine/engine/engine.h"
#include "engine/gba/registers/display/GBADisplayStatus.h"
#include "engine/gba/registers/input/GBAInput.h"
#include "engine/scene/SceneManager.h"

#include "game/scenes/Scene0.h"

#define VBLANK_SCNLNE_START 160

static void WaitForVSync();

int main()
{
	Engine engine;
	Scene0 scene0(&engine);
	SceneManager sceneManager(&scene0);
	GBA::OAMManager* oamManager = engine.GetOAMManager();

	// Test Initialisation		
	GBA::Input::Update();

	// Update loop
	while (true)
	{
		// General update
		GBA::Input::Update();

		sceneManager.UpdateScene(&engine);
		sceneManager.RenderScene(&engine);

		// Main update
		WaitForVSync();

		// Real Render
		oamManager->DoMasterRender(&engine);
	}

	return 0;
}

static void WaitForVSync()
{
	while (GBA::DisplayStatus::VCount() >= VBLANK_SCNLNE_START);   // wait till VDraw
	while (GBA::DisplayStatus::VCount() < VBLANK_SCNLNE_START);    // wait till VBlank
}
