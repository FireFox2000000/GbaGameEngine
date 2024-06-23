#include <memory>
#include "engine/engine/engine.h"
#include "engine/scene/SceneManager.h"
#include "engine/time/Time.h"
#include "engine/graphics/Graphics.h"
#include "engine/audio/AudioManager.h"
#include "engine/io/FileSystem.h"
#include "engine/physics/PhysicsConfig.h"
#include "engine/debug/Profiler.h"

#include "engine/gba/interrupts/GBAInterruptSwitchboard.h"
#include "gbatek/Bios.h"

#include "engine/input/InputManager.h"

#include "engine/input/GbaKeypadMap.h"
#include "game/input/Input.h"
#include "game/input/InputConfig.h"
#include "game/scenes/LevelSelectorScene.h"

#include "game/data/FileRegistry.h"

#include "gbatek/DisplayStatus.h"
#include "gbatek/Interrupts.h"

static void RegisterInterrupts();

#define VBLANK_SCNLNE_START 160
//#define DEBUG_COLLIDERS

#ifdef DEBUG_COLLIDERS
#include "engine/debug/DebugRender.h"
#endif

int main()
{
	GBA::InterruptSwitchboard::Init();
	RegisterInterrupts();

	Engine& engine = Engine::GetInstance();
	std::unique_ptr<FileRegistry> fileRegistry = std::make_unique<FileRegistry>();
	Input::InputManager* inputManager = engine.GetComponent<Input::InputManager>();

#ifdef DEBUG_COLLIDERS
	DebugRender m_debugRenderer;	// TODO, should move this onto engine?
#endif

	IO::FileSystem* fileSystem = engine.GetComponent<IO::FileSystem>();
	fileSystem->SetRegistry(fileRegistry.get());

	Time* time = engine.GetComponent<Time>();
	AudioManager* audioManager = engine.GetComponent<AudioManager>();

	// Input Initialisation	
	inputManager->Update();

	time->Start();
	DEBUG_LOG("Engine initialised");

	SceneManager* sceneManager = engine.GetComponent<SceneManager>();
	sceneManager->ChangeScene<LevelSelectorScene>();

	s32 timeToNextFixedUpdateMicroSeconds = 0;
	constexpr u32 fixedUpdateDtMicroseconds = SECONDS_TO_MICROSECONDS(PhysicsConfig::c_fixedUpdateRate.ToFloat());

	// Update loop
	while (true)
	{
		// Main update
		GBATEK::Bios::VBlankInterruptWait();

		// VBlank, must be under 83776 cycles no matter what
		{
			PROFILE_SCOPED(Profile_VBlank);

			sceneManager->RenderScene();
		}

		// VDraw, should aim to be under 197120 cycles to target 60 fps. Can go beyond this for 30 fps
		{
			PROFILE_SCOPED(Profile_VDraw);

			// End frame
			{
				audioManager->Update();

				if (Input::GetInputDown(GameInputs::SoftReset, inputManager->GetDevices()))
				{
					engine.GetComponent<Graphics>()->PrepareForSceneChange();
					sceneManager->Dispose();
					GBATEK::Bios::SoftReset();
				}

				if (sceneManager->HasSceneChangeQueued())
				{
					engine.GetComponent<Graphics>()->PrepareForSceneChange();
					sceneManager->EnterQueuedScene();
				}

				// Calculate dt between frames
				time->Advance();
			}

			// Start frame
			{
				// General update
				inputManager->Update();

				sceneManager->UpdateScene();

				const u32 dtMicroSeconds = time->GetDtTimeValue().TotalMicroseconds();

				timeToNextFixedUpdateMicroSeconds += dtMicroSeconds;
				while (timeToNextFixedUpdateMicroSeconds > (s32)fixedUpdateDtMicroseconds)
				{
					timeToNextFixedUpdateMicroSeconds -= fixedUpdateDtMicroseconds;

					// Perform fixed update
					sceneManager->FixedUpdateScene();
				}

				audioManager->Update();

#ifdef DEBUG_COLLIDERS
				m_debugRenderer.RenderColliders(engine.get(), sceneManager->GetCurrentScene()->GetMainCamera());
#endif

				sceneManager->PreRenderScene();

				audioManager->Update();

				auto* entityManager = engine.GetEntityRegistry();
				entityManager->InternalFinaliseDestroy();
			}
		}
	}

	sceneManager->Dispose();

	return 0;
}

void EnableVBlankIntr()
{
	GBATEK::ioRegisterDisplayStatus->vBlankInterruptRequestEnabled = true;
	GBATEK::ioRegisterInterruptEnable->vBlank = true;
}

void RegisterInterrupts()
{
	GBATEK::ioRegisterInterruptMasterEnable->status = GBATEK::InterruptMasterEnable::Status::DisableAll;
	EnableVBlankIntr();
	GBATEK::ioRegisterInterruptMasterEnable->status = GBATEK::InterruptMasterEnable::Status::InterruptEnableRegister;
}
