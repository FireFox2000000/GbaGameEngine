#include <memory>
#include "engine/engine/engine.h"
#include "engine/animation/SpriteAnimator.h"
#include "engine/scene/SceneManager.h"
#include "engine/time/Time.h"

#include "engine/gba/registers/display/GBADisplayStatus.h"
#include "engine/gba/registers/input/GBAInput.h"
#include "engine/gba/registers/clock/GBATimer.h"

#include "engine/gba/graphics/oam/GBAOAMManager.h"

#include "game/scenes/Scene0.h"

#include "engine/debug/DebugLog.h"

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

#ifdef TEST_PROFILING
	// Test profiling
	GBA::Timer profilerClock(2);
	profilerClock.SetFrequency(GBA::Timer::Cycle_256);
#endif
	DEBUG_LOG("Engine initialised");

	// Update loop
	while (true)
	{
#ifdef TEST_PROFILING
		profilerClock.SetActive(true);
#endif
		// VDraw should have started before this, main loop should aim to be under 197120 cycles

		// General update
		GBA::Input::Update();

		sceneManager->UpdateScene(engine.get());

		System::SpriteAnimator::Update(engine.get());
		sceneManager->RenderScene(engine.get());
#ifdef TEST_PROFILING
		DEBUG_LOGFORMAT("[Profile update + render scene] = %d", profilerClock.GetCurrentTimerCount());
		profilerClock.SetActive(false);
#endif
		// Main update
		WaitForVSync();

		// VBlank, should be under 83776 cycles
#ifdef TEST_PROFILING
		profilerClock.SetActive(true);
#endif
		oamManager->DoMasterRenderIntoMemory(engine.get());
#ifdef TEST_PROFILING
		DEBUG_LOGFORMAT("[Profile DoMasterRenderIntoMemory] = %d", profilerClock.GetCurrentTimerCount());
		profilerClock.SetActive(false);
#endif

		engine->EditComponent<Time>()->Advance();
	}

	return 0;
}

static void WaitForVSync()
{
	while (GBA::DisplayStatus::VCount() >= VBLANK_SCNLNE_START);   // wait till VDraw
	while (GBA::DisplayStatus::VCount() < VBLANK_SCNLNE_START);    // wait till VBlank
}
