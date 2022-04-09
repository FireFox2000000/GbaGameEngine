#include "TilemapTestScene.h"
#include "engine/engine/engine.h"
#include "engine/gba/registers/input/GBAInput.h"
#include "engine/render/TilemapRenderer.h"
#include "engine/gameobject/transformation/Transform.h"
#include "engine/gameobject/Camera.h"
#include "engine/asset/AssetLoadFunctions.h"
#include "engine/graphics/Graphics.h"
#include "engine/time/Time.h"
#include "engine/io/FileSystem.h"
#include "engine/scene/SceneManager.h"
#include "game/scenes/LevelSelectorScene.h"
#include "engine/graphics/GraphicsSetup.h"
#include "game/input/Input.h"

TilemapTestScene::TilemapTestScene(Engine* engine) : Scene(engine)
{
}

void TilemapTestScene::Enter(Engine * engine)
{
	GraphicsSetup::InitialiseStandardGraphics();

	IO::FileSystem* fileSystem = engine->GetComponent<IO::FileSystem>();

	FilePtr uiAtlusFile = fileSystem->Open("tilemaps/UiAtlus");
	m_uiRenderer.LoadAtlus(uiAtlusFile);

	m_uiRenderCommandQueue.Enque([this] { m_uiRenderer.RenderText("Hello World!", Vector2<int>(1, 1)); });

	// Create a tilemap asset
	m_assetManager.AddTilemapSetFromFile(TilemapSetID::NightSky, fileSystem->Open("tilemaps/NightSky"));
	m_assetManager.AddTilemapSetFromFile(TilemapSetID::NightSkyInverted, fileSystem->Open("tilemaps/NightSky_Inverted"));
	m_assetManager.AddTilemapSetFromFile(TilemapSetID::Eosd, fileSystem->Open("tilemaps/Eosd"));

	Tilemap* tilemap = m_assetManager.GetTilemap(TilemapSetID::NightSky, 0);

	// Load the tilemap into vram
	Graphics* graphicsManager = engine->GetComponent<Graphics>();
	graphicsManager->LoadTilemap(*tilemap);

	m_background = m_gameObjects.AddNew();
	Component::TilemapRenderer& tilemapRenderer = m_background->AddComponent<Component::TilemapRenderer>();
	tilemapRenderer.SetTilemap(tilemap);
	tilemapRenderer.SetVisible(true);

	Component::Transform* transform = m_mainCamera.EditComponent<Component::Transform>();
	transform->SetPosition(-17 + 20, 6);

	Graphics* gfx = engine->GetComponent<Graphics>();
	std::shared_ptr<GfxScreenFadeIn> fadeTask = std::make_shared<GfxScreenFadeIn>(Colour::White, 0.5f);
	if (gfx->KickPostProcessingGfxTask(fadeTask))
	{
		m_kickedFadeInTask = fadeTask;
	}

	GBA::DMG::EnableSoundChannels(GBA::DMG::SoundChannels::Sound1 | GBA::DMG::SoundChannels::Sound2 | GBA::DMG::SoundChannels::Sound4);
	GBA::DMG::SetMasterVolume(1.0f);

	m_fallOfFallMidi = std::make_unique<GBA::DMG::Midi::Player>(fileSystem->Open("audio/DmgMidiFallOfFall"));
}

void TilemapTestScene::Exit(Engine * engine)
{
	// Ideally all maps should be turned off by now unless we're doing fancy transitions or something. 
	m_assetManager.Dispose(engine);

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

void TilemapTestScene::Update(Engine * engine)
{
	const Time* time = engine->GetComponent<Time>();
	tFixedPoint24 dt = time->GetDt();

	Component::Transform* transform = m_mainCamera.EditComponent<Component::Transform>();
	
	auto position = transform->GetPosition();
	const float speed = 1 * dt.ToFloat();
	//position.x -= speed;
	//transform->SetPosition(position.x, position.y);

	position.x += speed;

	Input::InputManager* inputManager = engine->GetComponent<Input::InputManager>();
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
		GBA::DMG::Test();
		
		Graphics* gfx = engine->GetComponent<Graphics>();
		std::shared_ptr<GfxScreenFadeOut> fadeTask = std::make_shared<GfxScreenFadeOut>(Colour::Black, 0.5f);
		if (gfx->KickPostProcessingGfxTask(fadeTask))
		{
			m_kickedFadeOutTask = fadeTask;
		}
	}

	if (m_kickedFadeOutTask && m_kickedFadeOutTask->IsComplete())
	{
		m_kickedFadeOutTask = nullptr;

		SceneManager* sceneManager = engine->GetComponent<SceneManager>();
		sceneManager->ChangeScene<LevelSelectorScene>(engine);
	}

	m_stateMachine.Update();
}

void TilemapTestScene::Render(Engine * engine)
{
	Scene::Render(engine);

	m_uiRenderCommandQueue.ExecuteCommands();
}
