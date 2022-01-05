#include <memory>
#include "engine/engine/engine.h"
#include "engine/scene/SceneManager.h"
#include "engine/time/Time.h"
#include "engine/graphics/Graphics.h"
#include "engine/audio/AudioManager.h"
#include "engine/io/FileSystem.h"

#include "engine/gba/registers/display/GBADisplayStatus.h"
#include "engine/gba/registers/input/GBAInput.h"
#include "engine/gba/interrupts/GBAInterruptSwitchboard.h"
#include "engine/gba/bios/GBABios.h"
#include "engine/gba/registers/input/GBAInput.h"

#include "engine/input/InputManager.h"

#include "engine/input/GbaKeypadMap.h"
#include "game/input/Input.h"
#include "game/input/InputConfig.h"
#include "game/scenes/LevelSelectorScene.h"

#include "game/data/FileRegistry.h"

static void RegisterInterrupts();

#define VBLANK_SCNLNE_START 160
//#define TEST_PROFILING
//#define DEBUG_COLLIDERS

#ifdef DEBUG_COLLIDERS
#include "engine/debug/DebugRender.h"
#endif

int main()
{
	GBA::InterruptSwitchboard::Init();
	RegisterInterrupts();
	GBA::Interrupts::EnableInterrupts();

	std::unique_ptr<Engine> engine = std::make_unique<Engine>();
	std::unique_ptr<FileRegistry> fileRegistry = std::make_unique<FileRegistry>();
	Input::InputManager* inputManager = engine->GetComponent<Input::InputManager>();

#ifdef DEBUG_COLLIDERS
	DebugRender m_debugRenderer;	// TODO, should move this onto engine?
#endif

	IO::FileSystem* fileSystem = engine.get()->GetComponent<IO::FileSystem>();
	fileSystem->SetRegistry(fileRegistry.get());

	Time* time = engine->GetComponent<Time>();
	AudioManager* audioManager = engine->GetComponent<AudioManager>();

	// Input Initialisation	
	inputManager->Update();

	time->Start();
	DEBUG_LOG("Engine initialised");

	SceneManager* sceneManager = engine.get()->GetComponent<SceneManager>();
	sceneManager->ChangeScene<LevelSelectorScene>(engine.get());

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
			inputManager->Update();

			sceneManager->UpdateScene(engine.get());

			audioManager->Update();

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
		GBA::Bios::VBlankIntrWait();

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

		if (Input::GetInputDown(GameInputs::SoftReset, inputManager->GetDevices()))
		{
			GBA::Bios::SoftReset();
			//sceneManager->ChangeScene<LevelSelectorScene>(engine.get());
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

void EnableVBlankIntr()
{
	GBA::DisplayStatus::EnableVBlankInterrupts();
	GBA::Interrupts::EnableInterrupt(GBA::Interrupts::VBlank);
}

void RegisterInterrupts()
{
	GBA::Interrupts::DisableInterrupts();

	EnableVBlankIntr();

	GBA::Interrupts::EnableInterrupts();
}
