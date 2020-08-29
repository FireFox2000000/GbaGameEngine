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

#define DYNAMIC_MAP_TEST

TilemapTestScene::TilemapTestScene(Engine * engine) : Scene(engine)
{
}

void TilemapTestScene::Enter(Engine * engine)
{
	using namespace GBA;
	using namespace GBA::DisplayOptions;

	DisplayControl::SetDisplayOptions(Mode0 | Sprites | MappingMode1D);

	ECS::EntityComponentManager* entityManager = engine->GetEntityRegistry();

	TilemapLibrary* tilemapLib = engine->EditComponent<TilemapLibrary>();
	Tilemap* tilemap = tilemapLib->GetTilemap(TilemapSetID::eosd, 0);

	// Load the tilemap into vram
	TilemapManager* tilemapManager = engine->EditComponent<TilemapManager>();

#ifdef DYNAMIC_MAP_TEST
	tilemapManager->LoadDynamicMap(*tilemap);
#else
	tilemapManager->LoadStaticMap(*tilemap);
#endif

	m_loadedTilemaps.Add(tilemap);

	GameObject* background = m_gameObjects.AddNew(entityManager);
	Component::TilemapRenderer& tilemapRenderer = background->AddComponent<Component::TilemapRenderer>();
	tilemapRenderer.SetTilemap(tilemap);
	tilemapRenderer.SetVisible(true);
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
	auto* entityManager = engine->GetEntityRegistry();
	
	entityManager->InvokeEach<Component::Transform, Component::Camera>(
		[]
	(Component::Transform& transform, Component::Camera& camera)
		{
#ifdef DYNAMIC_MAP_TEST
			transform.SetPosition(15, -6);
#else
			auto position = transform.GetPosition();
			const float speed = 0.05f;
			position.x += speed;
			transform.SetPosition(position);
#endif
			/*
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
			}*/
		});
}

#include "engine/gba/graphics/tiles/GBATile.h"

void SetBackgroundTile(const Tilemap* tilemap, u16 data, int row, int column)
{
	row %= TilemapManager::VARIABLE_TILEMAP_SIZE.x;
	column %= TilemapManager::VARIABLE_TILEMAP_SIZE.y;

	u32 offset = column * TilemapManager::VARIABLE_TILEMAP_SIZE.x + row;

	GBA::Vram::GetInstance().SetBackgroundTileData(tilemap->GetMapScreenBaseBlockIndex(), offset, data);
}

void TilemapTestScene::Render(Engine * engine)
{
	Scene::Render(engine);

#ifdef DYNAMIC_MAP_TEST
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
	}
#else
	
#endif
}
