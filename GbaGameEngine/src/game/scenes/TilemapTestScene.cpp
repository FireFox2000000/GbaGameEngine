#include "TilemapTestScene.h"
#include "engine/engine/engine.h"
#include "engine/gba/registers/display/GBADisplayControl.h"
#include "engine/gba/registers/input/GBAInput.h"
#include "engine/asset/libraries/TilemapLibrary.h"
#include "engine/render/TilemapRenderer.h"
#include "engine/gameobject/transformation/Transform.h"
#include "engine/gameobject/Camera.h"

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
	Tilemap* testBg = tilemapLib->GetTilemap(TilemapSetID::eosd, 0);

	GameObject* background = m_gameObjects.AddNew(entityManager);
	Component::TilemapRenderer& tilemapRenderer = background->AddComponent<Component::TilemapRenderer>();
	tilemapRenderer.SetTilemap(testBg);
	tilemapRenderer.AssignBackgroundSlot();
}

void TilemapTestScene::Update(Engine * engine)
{
	auto* entityManager = engine->GetEntityRegistry();

	entityManager->InvokeEach<Component::Transform, Component::Camera>(
		[]
	(Component::Transform& transform, Component::Camera& camera)
		{
			auto position = transform.GetPosition();
			const float speed = 0.05f;
			position.x += speed;
			transform.SetPosition(position);
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
