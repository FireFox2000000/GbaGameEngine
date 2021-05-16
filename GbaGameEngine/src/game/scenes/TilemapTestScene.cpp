#include "TilemapTestScene.h"
#include "engine/engine/engine.h"
#include "engine/gba/registers/display/GBADisplayControl.h"
#include "engine/gba/registers/input/GBAInput.h"
#include "engine/render/TilemapRenderer.h"
#include "engine/gameobject/transformation/Transform.h"
#include "engine/gameobject/Camera.h"
#include "engine/graphicalassets/tilemap/TilemapManager.h"
#include "engine/graphicalassets/tilemap/TilemapLoadFunctions.h"

#include "engine/gba/graphics/vram/GBAVram.h"
#include "engine/time/Time.h"
#include "game/data/eosd_bin.h"

TilemapTestScene::TilemapTestScene(Engine * engine) : Scene(engine)
{
}

void TilemapTestScene::Enter(Engine * engine)
{
	using namespace GBA;
	using namespace GBA::DisplayOptions;

	DisplayControl::SetDisplayOptions(Mode0 | Sprites | MappingMode1D);

	// Create a tilemap asset
	m_tilemapSets.Add(TilemapLoadFunctions::CreateTilemapSetFromFile(eosd_bin::data));
	Tilemap* tilemap = m_tilemapSets[0].GetTilemap(0);

	// Load the tilemap into vram
	TilemapManager* tilemapManager = engine->EditComponent<TilemapManager>();
	tilemapManager->LoadTilemap(*tilemap);
	m_loadedTilemaps.Add(tilemap);

	GameObject* background = m_gameObjects.AddNew();
	Component::TilemapRenderer& tilemapRenderer = background->AddComponent<Component::TilemapRenderer>();
	tilemapRenderer.SetTilemap(tilemap);
	tilemapRenderer.SetVisible(true);

	Component::Transform* transform = m_mainCamera.EditComponent<Component::Transform>();
	transform->SetPosition(-17 - 20, 6);
}

void TilemapTestScene::Exit(Engine * engine)
{
	// Ideally all maps should be turned off by now unless we're doing fancy transitions or something. 
	TilemapManager* tilemapManager = engine->EditComponent<TilemapManager>();
	for (auto& tilemapSet : m_tilemapSets)
	{
		for (auto& tilemap : tilemapSet.m_maps)
		{
			tilemapManager->Unload(&tilemap);
		}
	}
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
