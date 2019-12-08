#include <memory>
#include "engine/engine/engine.h"
#include "engine/animation/SpriteAnimator.h"
#include "engine/scene/SceneManager.h"
#include "engine/time/Time.h"

#include "engine/gba/registers/display/GBADisplayStatus.h"
#include "engine/gba/registers/input/GBAInput.h"
#include "engine/gba/graphics/oam/GBAOAMManager.h"

#include "game/scenes/Scene0.h"

#define VBLANK_SCNLNE_START 160

static void WaitForVSync();

int main()
{
	std::unique_ptr<Engine> engine = std::make_unique<Engine>();

	SceneManager* sceneManager = engine.get()->EditComponent<SceneManager>();
	sceneManager->ChangeScene<Scene0>(engine.get());

	GBA::OAMManager* oamManager = engine->EditComponent<GBA::OAMManager>();

	// Test Initialisation		
	GBA::Input::Update();

	// Update loop
	while (true)
	{
		// General update
		GBA::Input::Update();

		sceneManager->UpdateScene(engine.get());

		System::SpriteAnimator::Update(engine.get());

		sceneManager->RenderScene(engine.get());
		
		// Main update
		WaitForVSync();
		
		// Real Render
		oamManager->DoMasterRenderIntoMemory(engine.get());
		
		engine->EditComponent<Time>()->Advance();
	}

	return 0;
}

static void WaitForVSync()
{
	while (GBA::DisplayStatus::VCount() >= VBLANK_SCNLNE_START);   // wait till VDraw
	while (GBA::DisplayStatus::VCount() < VBLANK_SCNLNE_START);    // wait till VBlank
}
