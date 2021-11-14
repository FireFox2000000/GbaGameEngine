#include <memory>
#include "engine/engine/engine.h"
#include "engine/animation/SpriteAnimator.h"
#include "engine/scene/SceneManager.h"
#include "engine/time/Time.h"
#include "engine/graphicalassets/Graphics.h"

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

	bool sceneFlipped = false;

	// Update loop
	while (true)
	{
		// VDraw, should aim to be under 197120 cycles to target 60 fps. Can go beyond this for 30 fps
		{
#ifdef TEST_PROFILING
			auto profileStart = Time::CaptureSystemTimeSnapshot();
#endif
			// General update
			GBA::Input::Update();

			sceneManager->UpdateScene(engine.get());

			System::SpriteAnimator::Update(engine.get());

			sceneManager->PreRenderScene(engine.get());
#ifdef TEST_PROFILING
			auto profileStop = Time::CaptureSystemTimeSnapshot();
			u32 profileResult = (profileStop.TotalCycles_Freq1024() - profileStart.TotalCycles_Freq1024()) * 1024;
			DEBUG_LOGFORMAT("[Profile VDraw] = %d", profileResult);
#endif
		}

		// Main update
		WaitForVSync();

		// VBlank, should be under 83776 cycles no matter what
		{
#ifdef TEST_PROFILING
			auto profileStart = Time::CaptureSystemTimeSnapshot();
#endif
			sceneManager->RenderScene(engine.get());
#ifdef TEST_PROFILING
			auto profileStop = Time::CaptureSystemTimeSnapshot();
			u32 profileResult = (profileStop.TotalCycles_Freq1024() - profileStart.TotalCycles_Freq1024()) * 1024;
			DEBUG_LOGFORMAT("[Profile VBlank] = %d", profileResult);
#endif
		}

		if (GBA::Input::GetKeyDown(GBA::Buttons::Start))
		{
			if (sceneFlipped)
				sceneManager->ChangeScene<Scene0>(engine.get());
			else
				sceneManager->ChangeScene<TilemapTestScene>(engine.get());

			sceneFlipped = !sceneFlipped;
		}

		if (sceneManager->HasSceneChangeQueued())
		{
			engine.get()->EditComponent<Graphics>()->PrepareForSceneChange();
			sceneManager->EnterQueuedScene(engine.get());
		}

		// Calculate dt between frames
		time->Advance();
	}

	return 0;
}

static void WaitForVSync()
{
	while (GBA::DisplayStatus::VCount() >= VBLANK_SCNLNE_START);   // wait till VDraw
	while (GBA::DisplayStatus::VCount() < VBLANK_SCNLNE_START);    // wait till VBlank
}
