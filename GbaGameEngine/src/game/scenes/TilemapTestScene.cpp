#include "TilemapTestScene.h"
#include "engine/engine/engine.h"
#include "engine/gba/registers/display/GBADisplayControl.h"
#include "engine/gba/registers/input/GBAInput.h"
#include "engine/asset/libraries/TilemapLibrary.h"
#include "engine/render/TilemapRenderer.h"
#include "engine/gameobject/transformation/Transform.h"
#include "engine/gameobject/Camera.h"
#include "engine/graphicalassets/tilemap/TilemapManager.h"
#include "engine/gba/graphics/vram/GBAVram.h"
#include "engine/time/Time.h"

TilemapTestScene::TilemapTestScene(Engine * engine) : Scene(engine)
{
}

void TilemapTestScene::Enter(Engine * engine)
{
	using namespace GBA;
	using namespace GBA::DisplayOptions;

	DisplayControl::SetDisplayOptions(Mode0 | Sprites | MappingMode1D);

	TilemapLibrary* tilemapLib = engine->EditComponent<TilemapLibrary>();
	Tilemap* tilemap = tilemapLib->GetTilemap(TilemapSetID::eosd, 0);

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
	for (Tilemap* map : m_loadedTilemaps)
	{
		tilemapManager->Unload(map);
	}
}

void TilemapTestScene::Update(Engine * engine)
{
	Component::Transform* transform = m_mainCamera.EditComponent<Component::Transform>();
	
	auto position = transform->GetPosition();
	const float speed = 0.05f;
	//position.x -= speed;
	//transform->SetPosition(position.x, position.y);

	
	if (GBA::Input::GetKey(GBA::Buttons::Left))
	{
		position.x += -speed;
	}

	if (GBA::Input::GetKey(GBA::Buttons::Right))
	{
		position.x += speed;
	}

	if (GBA::Input::GetKey(GBA::Buttons::Down))
	{
		position.y += -speed;
	}

	if (GBA::Input::GetKey(GBA::Buttons::Up))
	{
		position.y += speed;
	}

	transform->SetPosition(position.x, position.y);
}

#include "engine/gba/graphics/tiles/GBATile.h"

//void SetBackgroundTile(const Tilemap* tilemap, u16 data, int row, int column)
//{
//	row %= TilemapManager::VARIABLE_TILEMAP_SIZE.x;
//	column %= TilemapManager::VARIABLE_TILEMAP_SIZE.y;
//
//	u32 offset = column * TilemapManager::VARIABLE_TILEMAP_SIZE.x + row;
//
//	GBA::Vram::GetInstance().SetBackgroundTileData(tilemap->GetMapScreenBaseBlockIndex(), offset, data);
//}

void TilemapTestScene::Render(Engine * engine)
{
	Scene::Render(engine);

#ifdef DYNAMIC_MAP_TEST
	/*
	using namespace GBA;
	TilemapLibrary* tilemapLib = engine->EditComponent<TilemapLibrary>();
	Tilemap* tilemap = tilemapLib->GetTilemap(TilemapSetID::eosd, 0);
	const auto& size = TilemapManager::VARIABLE_TILEMAP_SIZE;// tilemap->GetSizeInTiles();

	const int c_tilePerFrame = 10;
	for (int i = 0; i < c_tilePerFrame; ++i)
	{
		if (tilemapDataIndex >= tilemap->GetTileMapLength())
		{
			return;
		}

		if (row >= size.x)
		{
			row = 0;
			++column;
		}

		if (column >= size.y)
		{
			column = 0;
		}

		// Todo, load tiles in one by one I guess
		{
			u16 tileInfo = tilemap->GetTileMapData()[tilemapDataIndex];
			SetBackgroundTile(tilemap, tileInfo, row, column);
			++tilemapDataIndex;
			++row;
		}
	}*/
#else
	
#endif
}
