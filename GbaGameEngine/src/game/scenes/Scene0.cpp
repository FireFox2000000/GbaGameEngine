#include "Scene0.h"
#include "engine/engine/engine.h"
#include "engine/gba/registers/display/GBADisplayControl.h"
#include "engine/render/SpriteRenderer.h"
#include "engine/graphicalassets/sprite/SpriteManager.h"
#include "engine/graphicalassets/sprite/SpriteLibrary.h"

#include "game/scripts/MovementTest.h"
#include "game/scripts/AnimationTest.h"
#include "engine/gameobject/transformation/Position.h"

Scene0::Scene0(Engine* engine)
	: Scene(engine)
{
	using namespace GBA;
	using namespace GBA::DisplayOptions;

	int totalTestSprites = 40;

	DisplayControl::SetDisplayOptions(Mode0 | Sprites | MappingMode1D);

	SpriteLibrary* spriteLibrary = engine->GetSpriteManager()->GetSpriteLibrary();
	ECS::EntityComponentManager* entityManager = engine->GetEntityRegistry();

	m_gameObjects.Reserve(totalTestSprites);
	
	if (true)
	{
		for (int i = 1; i < totalTestSprites; ++i)
		{
			GameObject* testBackgroundObject = m_gameObjects.AddNew(entityManager);
			
			Component::Position* position = testBackgroundObject->EditComponent<Component::Position>();
			position->x = (i * 0.2f) - 5;
			position->y = (i * 0.2f) - 5;

			Component::SpriteRenderer& testBackgroundRenderer = testBackgroundObject->AddComponent<Component::SpriteRenderer>();
			Sprite* shantae0 = spriteLibrary->GetSprite(SpriteAtlusID::Shantae_Idle, 0);
			testBackgroundRenderer.SetSprite(shantae0);

			testBackgroundObject->AddComponent<Component::AnimationTest>(engine);
		}
	}

	// Entt is iterating these backwards when rendering sprites. Add player movement one last to draw on top for now. 
	{
		GameObject* testObject = m_gameObjects.AddNew(entityManager);
		Component::Position* position = testObject->EditComponent<Component::Position>();
		position->x = 0;
		position->y = 0;

		Component::PlayerMovement& playerMovement = testObject->AddComponent<Component::PlayerMovement>();
		playerMovement.moveSpeed = 8.0f;

		Component::SpriteRenderer& testRenderer = testObject->AddComponent<Component::SpriteRenderer>();
		testRenderer.SetSprite(spriteLibrary->GetSprite(SpriteAtlusID::Shantae_Idle, 6));

		testObject->AddComponent<Component::AnimationTest>(engine);
	}
}

Scene0::~Scene0()
{
}

void Scene0::Update(Engine* engine)
{
	System::PlayerMovement::Update(engine);
	System::AnimationTest::Update(engine);
	Scene::Update(engine);
}
