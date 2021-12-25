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
#include "game/scenes/UiAtlusTestScene.h"

#define VBLANK_SCNLNE_START 160
//#define TEST_PROFILING
//#define DEBUG_COLLIDERS

#ifdef DEBUG_COLLIDERS
#include "engine/debug/DebugRender.h"
#endif

static void WaitForVSync();

int main()
{
	std::unique_ptr<Engine> engine = std::make_unique<Engine>();

#ifdef DEBUG_COLLIDERS
	DebugRender m_debugRenderer;	// TODO, should move this onto engine?
#endif

	SceneManager* sceneManager = engine.get()->GetComponent<SceneManager>();
	sceneManager->ChangeScene<UiAtlusTestScene>(engine.get());

	Time* time = engine->GetComponent<Time>();
	AudioManager* audioManager = engine->GetComponent<AudioManager>();

	// Test Initialisation		
	GBA::Input::Update();

	time->Start();
	DEBUG_LOG("Engine initialised");

	bool sceneFlipped = false;

#ifdef TEST_PROFILING
	auto profileStart = Time::CaptureSystemTimeSnapshot();
#endif

	// Update loop
	while (true)
	{
		//auto systemTime = Time::CaptureSystemTimeSnapshot();
		//DEBUG_LOGFORMAT("Current time = (%d, %d)", systemTime.systemClockCount1, systemTime.systemClockCount2);

		// VDraw, should aim to be under 197120 cycles to target 60 fps. Can go beyond this for 30 fps
		{
			// General update
			GBA::Input::Update();

			sceneManager->UpdateScene(engine.get());

			audioManager->Update();

			System::SpriteAnimator::Update(engine.get());

#ifdef DEBUG_COLLIDERS
			m_debugRenderer.RenderColliders(engine.get(), sceneManager->GetCurrentScene()->GetMainCamera());
#endif

			sceneManager->PreRenderScene(engine.get());

#ifdef TEST_PROFILING
			auto profileStop = Time::CaptureSystemTimeSnapshot();
			u32 profileResult = (profileStop.TotalCycles() - profileStart.TotalCycles()) * Time::ClockFreq;
			PROFILE_LOGFORMAT("[Profile VDraw] = %d", profileResult);
#endif
		}

		audioManager->Update();

		// Main update
		WaitForVSync();

		// VBlank, must be under 83776 cycles no matter what
		{
#ifdef TEST_PROFILING
			profileStart = Time::CaptureSystemTimeSnapshot();
#endif
			sceneManager->RenderScene(engine.get());
#ifdef TEST_PROFILING
			auto profileStop = Time::CaptureSystemTimeSnapshot();
			u32 profileResult = (profileStop.TotalCycles() - profileStart.TotalCycles()) * Time::ClockFreq;
			PROFILE_LOGFORMAT("[Profile VBlank] = %d", profileResult);
#endif
		}

#ifdef TEST_PROFILING
		profileStart = Time::CaptureSystemTimeSnapshot();	// loops back around to profile the end of VDraw
#endif

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
			engine.get()->GetComponent<Graphics>()->PrepareForSceneChange();
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
