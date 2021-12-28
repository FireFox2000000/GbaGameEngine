#include "TilemapTestScene.h"
#include "engine/engine/engine.h"
#include "engine/gba/registers/display/GBADisplayControl.h"
#include "engine/gba/registers/input/GBAInput.h"
#include "engine/render/TilemapRenderer.h"
#include "engine/gameobject/transformation/Transform.h"
#include "engine/gameobject/Camera.h"
#include "engine/asset/AssetLoadFunctions.h"
#include "engine/graphicalassets/Graphics.h"
#include "engine/time/Time.h"
#include "engine/io/FileSystem.h"

#include "game/data/tilemaps/UiAtlus.h"

TilemapTestScene::TilemapTestScene(Engine * engine) : Scene(engine)
{
}

void TilemapTestScene::Enter(Engine * engine)
{
	using namespace GBA::DisplayOptions;

	GBA::DisplayControl::SetDisplayOptions(Mode0 | Sprites | MappingMode1D);

	IO::FileSystem* fileSystem = engine->GetComponent<IO::FileSystem>();

	FilePtr uiAtlusFile = fileSystem->Open("tilemaps/UiAtlus");
	m_uiRenderer.LoadAtlus(uiAtlusFile);
	m_uiRenderer.RenderText("Hello World!", Vector2<int>(1, 1));

	// Create a tilemap asset
	FilePtr nightSkyFile = fileSystem->Open("tilemaps/NightSky");
	m_assetManager.AddTilemapSetFromFile(TilemapSetID::Eosd, nightSkyFile);
	Tilemap* tilemap = m_assetManager.GetTilemap(TilemapSetID::Eosd, 0);

	// Load the tilemap into vram
	Graphics* graphicsManager = engine->GetComponent<Graphics>();
	graphicsManager->LoadTilemap(*tilemap);

	GameObject* background = m_gameObjects.AddNew();
	Component::TilemapRenderer& tilemapRenderer = background->AddComponent<Component::TilemapRenderer>();
	tilemapRenderer.SetTilemap(tilemap);
	tilemapRenderer.SetVisible(true);

	Component::Transform* transform = m_mainCamera.EditComponent<Component::Transform>();
	transform->SetPosition(-17 + 20, 6);
}

void TilemapTestScene::Exit(Engine * engine)
{
	// Ideally all maps should be turned off by now unless we're doing fancy transitions or something. 
	m_assetManager.Dispose(engine);
}

void TilemapTestScene::Update(Engine * engine)
{
	Component::Transform* transform = m_mainCamera.EditComponent<Component::Transform>();
	
	auto position = transform->GetPosition();
	const float speed = 1;
	//position.x -= speed;
	//transform->SetPosition(position.x, position.y);

	
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
	}

	transform->SetPosition(position.x, position.y);
}

void TilemapTestScene::Render(Engine * engine)
{
	Scene::Render(engine);
}
