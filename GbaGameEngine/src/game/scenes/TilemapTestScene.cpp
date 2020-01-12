#include "TilemapTestScene.h"
#include "engine/engine/engine.h"
#include "engine/gba/registers/display/GBADisplayControl.h"
#include "engine/gba/graphics/tiles/GBAPaletteBank.h"
#include "engine/gba/graphics/vram/GBAVram.h"
#include "engine/gba/registers/display/GBABackgroundControl.h"
#include "engine/gba/registers/input/GBAInput.h"
#include "engine/asset/libraries/TilemapLibrary.h"
#include "engine/render/TilemapRenderer.h"
#include "engine/gameobject/transformation/Transform.h"
#include "engine/gameobject/Camera.h"

TilemapTestScene::TilemapTestScene(Engine * engine) : Scene(engine)
{
}

void LoadTilemap(Tilemap* tilemap, GBA::BackgroundControl::Backgrounds backgroundSlot)
{
	using namespace GBA;

	if (backgroundSlot >= GBA::BackgroundControl::Count)
		return;

	TilemapSet* tilemapSet = tilemap->EditTilemapSet();

	// TODO- Ref tracking for each map we've loaded

	// Load tileset
	if (!tilemapSet->IsLoaded())
	{
		ColourPalette256 colourPalette(0);
		for (u32 i = 0; i < tilemapSet->m_paletteLength; ++i)
		{
			colourPalette[i] = tilemapSet->m_palette[i];
		}
		tilemapSet->m_paletteIndex = 0;

		PaletteBank::LoadBackgroundPalette(colourPalette);

		Vram::GetInstance().AllocBackgroundTileSetMem(tilemapSet->m_tileset, tilemapSet->m_tilesetLength, tilemapSet->m_tileSetCharacterBaseBlock);
	}

	// Load local map data
	if (!tilemap->IsLoaded())
	{
		Vram::GetInstance().AllocBackgroundTileMapMem(tilemap->m_tileMapData, tilemap->m_tileMapDataLength, tilemap->m_mapSbbIndex);
	}
}

void TilemapTestScene::Enter(Engine * engine)
{
	using namespace GBA;
	using namespace GBA::DisplayOptions;

	DisplayControl::SetDisplayOptions(Mode0 | Sprites | MappingMode1D);

	ECS::EntityComponentManager* entityManager = engine->GetEntityRegistry();

	TilemapLibrary* tilemapLib = engine->EditComponent<TilemapLibrary>();
	Tilemap* testBg = tilemapLib->GetTilemap(TilemapSetID::eosd, 0);

	GameObject* background = m_gameObjects.AddNew(entityManager);
	Component::TilemapRenderer& tilemapRenderer = background->AddComponent<Component::TilemapRenderer>();
	tilemapRenderer.SetTilemap(testBg);
	tilemapRenderer.AssignBackgroundSlot();

	BackgroundControl::Backgrounds bgId = tilemapRenderer.GetAssignedBackgroundSlot();
	LoadTilemap(testBg, bgId);
}

void TilemapTestScene::Update(Engine * engine)
{
	auto* entityManager = engine->GetEntityRegistry();

	entityManager->InvokeEach<Component::Transform, Component::Camera>(
		[]
	(Component::Transform& transform, Component::Camera& camera)
		{
			auto& position = transform.position;

			if (GBA::Input::GetKey(GBA::Buttons::Left))
			{
				position.x += -1;
			}

			if (GBA::Input::GetKey(GBA::Buttons::Right))
			{
				position.x += 1;
			}

			if (GBA::Input::GetKey(GBA::Buttons::Down))
			{
				position.y += -1;
			}

			if (GBA::Input::GetKey(GBA::Buttons::Up))
			{
				position.y += 1;
			}
		});
}
