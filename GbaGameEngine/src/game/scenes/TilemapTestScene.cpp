#include "TilemapTestScene.h"
#include "engine/engine/engine.h"
#include "engine/graphics/tilemap/TilemapRenderer.h"
#include "engine/transform/Transform2.h"
#include "engine/gameobject/Camera.h"
#include "engine/asset/AssetLoadFunctions.h"
#include "engine/graphics/Graphics.h"
#include "engine/time/Time.h"
#include "engine/io/FileSystem.h"
#include "engine/scene/SceneManager.h"
#include "game/scenes/LevelSelectorScene.h"
#include "engine/graphics/GraphicsSetup.h"
#include "game/input/Input.h"

#include "GBATEK/DMGSound.h"
#include "engine/gba/audio/GBADmgSound.h"

namespace
{
	void SetMasterVolume(float leftVolume, float rightVolume)
	{
		constexpr int DMG_VOL_MAX = 7;

		int dmgVolumeValueL = int(leftVolume * DMG_VOL_MAX + 0.5f);
		int dmgVolumeValueR = int(rightVolume * DMG_VOL_MAX + 0.5f);

		GBATEK::ioRegisterSoundControl->masterVolumeL = dmgVolumeValueL;
		GBATEK::ioRegisterSoundControl->masterVolumeR = dmgVolumeValueR;
	}

	void PlayTestSfx()
	{
		GBATEK::ioRegisterSoundControl->channel1LeftEnabled = GBATEK::DMGSoundEnable::Enabled;
		GBATEK::ioRegisterSoundControl->channel1RightEnabled = GBATEK::DMGSoundEnable::Enabled;

		SetMasterVolume(1.0f, 1.0f);

		// NR10_REG = 0x1E;
		GBATEK::ioRegisterSoundChannel1->sweep.number = 6;
		GBATEK::ioRegisterSoundChannel1->sweep.mode = GBATEK::SweepFrequencyDirection::Decrease;
		GBATEK::ioRegisterSoundChannel1->sweep.stepTime128Hz = 1;

		// NR11_REG = 0x10;
		// NR12_REG = 0xF3;
		GBATEK::SquareSound control;
		{
			control.soundLength = GBA::DMG::CalculateSoundLengthSeconds(0.1875f);	// sound1Control.soundLength = 16;
			control.waveDutyCycle = GBATEK::WaveDutyCycle::Cycle_1_8;
			control.envelopeStepTime = 3;
			control.envelopeDirection = GBATEK::EnvelopeStepDirection::Increase;
			control.envelopeInitialVolume = 7;
		}
		GBATEK::ioRegisterSoundChannel1->controlRegister.Write(control);

		// NR13_REG = 0x00;
		// NR14_REG = 0x87;
		GBATEK::ioRegisterSoundChannel1->frequencyRegister.Write(GBATEK::SoundFrequency{ GBA::DMG::FrequencyToRate(512), GBATEK::SustainMode::SoundLength, GBATEK::SoundReset::Reset });
	}
}

TilemapTestScene::TilemapTestScene() : Scene()
{
}

void TilemapTestScene::Enter()
{
	GraphicsSetup::InitialiseStandardGraphics();

	IO::FileSystem* fileSystem = Engine::GetInstance().GetComponent<IO::FileSystem>();
	ResourcesManager& resources = Engine::GetInstance().GetResourcesManager();

	MemoryMappedFileView uiAtlasFile = fileSystem->Open("tilemaps/UiAtlas");
	m_uiRenderer.LoadAtlas(uiAtlasFile);

	m_uiRenderCommandQueue.Enque([this] { m_uiRenderer.RenderText("Hello World!", Vector2i(1, 1)); });

	// Create a tilemap asset
	m_nightSkyTilemapSet = resources.LoadTilemapSet(fileSystem->Open("tilemaps/NightSkySet"));
	m_eosdTilemapSet = resources.LoadTilemapSet(fileSystem->Open("tilemaps/Eosd"));

	Tilemap* tilemap = m_nightSkyTilemapSet->GetTilemap(0);

	// Load the tilemap into vram
	Graphics* graphicsManager = Engine::GetInstance().GetComponent<Graphics>();
	graphicsManager->LoadTilemap(*tilemap);

	m_background = m_gameObjects.AddNew();
	Component::TilemapRenderer& tilemapRenderer = m_background->AddComponent<Component::TilemapRenderer>();
	tilemapRenderer.SetTilemap(tilemap);
	tilemapRenderer.SetVisible(true);

	Transform2* transform = m_mainCamera.EditComponent<Transform2>();
	transform->SetPosition(-17 + 20, 6);

	Graphics* gfx = Engine::GetInstance().GetComponent<Graphics>();
	std::shared_ptr<GfxScreenFadeIn> fadeTask = std::make_shared<GfxScreenFadeIn>(Colour::White, 0.5f);
	if (gfx->KickPostProcessingGfxTask(fadeTask))
	{
		m_kickedFadeInTask = fadeTask;
	}

	GBATEK::ioRegisterSoundControl->channel1LeftEnabled = GBATEK::DMGSoundEnable::Enabled;
	GBATEK::ioRegisterSoundControl->channel1RightEnabled = GBATEK::DMGSoundEnable::Enabled;

	GBATEK::ioRegisterSoundControl->channel2LeftEnabled = GBATEK::DMGSoundEnable::Enabled;
	GBATEK::ioRegisterSoundControl->channel2RightEnabled = GBATEK::DMGSoundEnable::Enabled;

	GBATEK::ioRegisterSoundControl->channel4LeftEnabled = GBATEK::DMGSoundEnable::Enabled;
	GBATEK::ioRegisterSoundControl->channel4RightEnabled = GBATEK::DMGSoundEnable::Enabled;

	SetMasterVolume(1.0f, 1.0f);

	m_fallOfFallMidi = std::make_unique<GBA::DMG::Midi::Player>(fileSystem->Open("audio/DmgMidiFallOfFall"));
}

void TilemapTestScene::Exit()
{
	// Ideally all maps should be turned off by now unless we're doing fancy transitions or something. 
	ResourcesManager& resources = Engine::GetInstance().GetResourcesManager();
	resources.Unload(m_nightSkyTilemapSet);
	resources.Unload(m_eosdTilemapSet);

	if (m_kickedFadeOutTask && !m_kickedFadeOutTask->IsComplete())
	{
		m_kickedFadeOutTask->Abort();
		m_kickedFadeOutTask = nullptr;
	}

	if (m_kickedFadeInTask && !m_kickedFadeInTask->IsComplete())
	{
		m_kickedFadeInTask->Abort();
		m_kickedFadeInTask = nullptr;
	}

	m_stateMachine.ChangeState(nullptr);
}

void TilemapTestScene::Update()
{
	const Time* time = Engine::GetInstance().GetComponent<Time>();
	FPi24 dt = time->GetDt();

	Transform2* transform = m_mainCamera.EditComponent<Transform2>();
	
	auto position = transform->GetPosition();

	// TODO - Why are we using float here? Overflow?
	const float speed = 1.0f * static_cast<float>(dt);
	//position.x -= speed;
	//transform->SetPosition(position.x, position.y);

	position.x += speed;

	Input::InputManager* inputManager = Engine::GetInstance().GetComponent<Input::InputManager>();
	const auto& devices = inputManager->GetDevices();

	m_fallOfFallMidi->Tick();

	/*
	if (GBA::Input::GetKeyDown(GBA::Buttons::Left))
	{
		position.x += -speed;
	}

	if (GBA::Input::GetKeyDown(GBA::Buttons::Right))
	{
		position.x += speed;
	}

	if (GBA::Input::GetKeyDown(GBA::Buttons::Down))
	{
		position.y += -speed;
	}

	if (GBA::Input::GetKeyDown(GBA::Buttons::Up))
	{
		position.y += speed;
	}*/

	transform->SetPosition(position.x, position.y);

	if (m_kickedFadeInTask && !m_kickedFadeInTask->IsComplete()) return;

	if (!m_kickedFadeOutTask && Input::GetInputDown(ExitTilemapTestScene, devices))
	{
		PlayTestSfx();
		
		Graphics* gfx = Engine::GetInstance().GetComponent<Graphics>();
		std::shared_ptr<GfxScreenFadeOut> fadeTask = std::make_shared<GfxScreenFadeOut>(Colour::Black, 0.5f);
		if (gfx->KickPostProcessingGfxTask(fadeTask))
		{
			m_kickedFadeOutTask = fadeTask;
		}
	}

	if (m_kickedFadeOutTask && m_kickedFadeOutTask->IsComplete())
	{
		m_kickedFadeOutTask = nullptr;

		SceneManager* sceneManager = Engine::GetInstance().GetComponent<SceneManager>();
		sceneManager->ChangeScene<LevelSelectorScene>();
	}

	m_stateMachine.Update();
}

void TilemapTestScene::Render()
{
	Scene::Render();

	m_uiRenderCommandQueue.ExecuteCommands();
}
