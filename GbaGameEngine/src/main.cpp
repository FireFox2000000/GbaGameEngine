#include <memory>
#include "engine/engine/engine.h"
#include "engine/animation/SpriteAnimator.h"
#include "engine/scene/SceneManager.h"
#include "engine/time/Time.h"
#include "engine/graphicalassets/Graphics.h"
#include "engine/audio/AudioManager.h"

#include "engine/gba/registers/display/GBADisplayStatus.h"
#include "engine/gba/registers/input/GBAInput.h"

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
	AudioManager* audioManager = engine->EditComponent<AudioManager>();

	// Test Initialisation		
	GBA::Input::Update();

	time->Start();
	DEBUG_LOG("Engine initialised");

	bool sceneFlipped = false;

	// Update loop
	while (true)
	{
		//auto systemTime = Time::CaptureSystemTimeSnapshot();
		//DEBUG_LOGFORMAT("Current time = (%d, %d)", systemTime.systemClockCount1, systemTime.systemClockCount2);

		// VDraw, should aim to be under 197120 cycles to target 60 fps. Can go beyond this for 30 fps
		{
#ifdef TEST_PROFILING
			auto profileStart = Time::CaptureSystemTimeSnapshot();
#endif
			// General update
			GBA::Input::Update();

			sceneManager->UpdateScene(engine.get());

			audioManager->Update();

			System::SpriteAnimator::Update(engine.get());

			sceneManager->PreRenderScene(engine.get());
#ifdef TEST_PROFILING
			auto profileStop = Time::CaptureSystemTimeSnapshot();
			u32 profileResult = (profileStop.TotalCycles() - profileStart.TotalCycles()) * Time::ClockFreq;
			DEBUG_LOGFORMAT("[Profile VDraw] = %d", profileResult);
#endif
		}

		audioManager->Update();

		// Main update
		WaitForVSync();

		// VBlank, must be under 83776 cycles no matter what
		{
#ifdef TEST_PROFILING
			auto profileStart = Time::CaptureSystemTimeSnapshot();
#endif
			sceneManager->RenderScene(engine.get());
#ifdef TEST_PROFILING
			auto profileStop = Time::CaptureSystemTimeSnapshot();
			u32 profileResult = (profileStop.TotalCycles() - profileStart.TotalCycles()) * Time::ClockFreq;
			DEBUG_LOGFORMAT("[Profile VBlank] = %d", profileResult);
#endif
		}

		audioManager->Update();

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
