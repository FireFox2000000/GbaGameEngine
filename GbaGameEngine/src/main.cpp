#include <memory>
#include "engine/engine/engine.h"
#include "engine/scene/SceneManager.h"
#include "engine/time/Time.h"
#include "engine/graphics/Graphics.h"
#include "engine/audio/AudioManager.h"
#include "engine/io/FileSystem.h"
#include "engine/physics/PhysicsConfig.h"

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

	s32 timeToNextFixedUpdateMicroSeconds = 0;
	constexpr u32 fixedUpdateDtMicroseconds = SECONDS_TO_MICROSECONDS(PhysicsConfig::c_fixedUpdateRate.ToFloat());

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

			const u32 dtMicroSeconds = time->GetDtTimeValue().TotalMicroseconds();

			timeToNextFixedUpdateMicroSeconds += dtMicroSeconds;
			while (timeToNextFixedUpdateMicroSeconds > (s32)fixedUpdateDtMicroseconds)
			{
				timeToNextFixedUpdateMicroSeconds -= fixedUpdateDtMicroseconds;

				// Perform fixed update
				sceneManager->FixedUpdateScene(engine.get());
			}

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

		auto* entityManager = engine->GetEntityRegistry();
		entityManager->InternalFinaliseDestroy();

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
			engine.get()->GetComponent<Graphics>()->PrepareForSceneChange();
			sceneManager->Dispose(engine.get());
			GBA::Bios::SoftReset();
		}

		if (sceneManager->HasSceneChangeQueued())
		{
			engine.get()->GetComponent<Graphics>()->PrepareForSceneChange();
			sceneManager->EnterQueuedScene(engine.get());
		}

		// Calculate dt between frames
		time->Advance();
	}

	sceneManager->Dispose(engine.get());

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
