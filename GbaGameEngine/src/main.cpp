#include <memory>
#include "engine/engine/engine.h"
#include "engine/animation/SpriteAnimator.h"
#include "engine/scene/SceneManager.h"
#include "engine/time/Time.h"

#include "engine/gba/registers/display/GBADisplayStatus.h"
#include "engine/gba/registers/input/GBAInput.h"
#include "engine/gba/registers/clock/GBATimer.h"

#include "game/scenes/Scene0.h"
#include "game/scenes/TilemapTestScene.h"

#define VBLANK_SCNLNE_START 160
//#define TEST_PROFILING
static void WaitForVSync();

int main()
{
	std::unique_ptr<Engine> engine = std::make_unique<Engine>();

	SceneManager* sceneManager = engine.get()->EditComponent<SceneManager>();
	sceneManager->ChangeScene<Scene0>(engine.get());

	Time* time = engine->EditComponent<Time>();

	// Test Initialisation		
	GBA::Input::Update();

	time->Start();
	DEBUG_LOG("Engine initialised");

	// Update loop
	while (true)
	{
		// VDraw should have started before this, main loop should aim to be under 197120 cycles to target 60 fps. Can go beyond this for 30 fps
#ifdef TEST_PROFILING
		auto& profilerClock = GBA::Timers::GetTimer(GBA::Timers::Profile);
		profilerClock.SetFrequency(GBA::Timers::Cycle_64);
		profilerClock.SetActive(true);
#endif
		// General update
		GBA::Input::Update();

		sceneManager->UpdateScene(engine.get());

		System::SpriteAnimator::Update(engine.get());

		sceneManager->PreRenderScene(engine.get());
#ifdef TEST_PROFILING
		DEBUG_LOGFORMAT("[Profile VDraw] = %d", profilerClock.GetCurrentTimerCount());
		profilerClock.SetActive(false);
#endif
		// Main update
		WaitForVSync();

		// VBlank, should be under 83776 cycles
#ifdef TEST_PROFILING
		profilerClock.SetActive(true);
#endif
		sceneManager->RenderScene(engine.get());
#ifdef TEST_PROFILING
		DEBUG_LOGFORMAT("[Profile VBlank] = %d", profilerClock.GetCurrentTimerCount());
		profilerClock.SetActive(false);
#endif
		time->Advance();
	}

	return 0;
}

static void WaitForVSync()
{
	while (GBA::DisplayStatus::VCount() >= VBLANK_SCNLNE_START);   // wait till VDraw
	while (GBA::DisplayStatus::VCount() < VBLANK_SCNLNE_START);    // wait till VBlank
}
