#include "Scene0.h"
#include "engine/engine/engine.h"
#include "engine/gba/registers/display/GBADisplayControl.h"
#include "engine/render/SpriteRenderer.h"
#include "engine/graphicalassets/sprite/SpriteManager.h"
#include "engine/graphicalassets/sprite/SpriteLibrary.h"

#include "game/scripts/MovementTest.h"
#include "engine/animation/SpriteAnimator.h"
#include "engine/gameobject/transformation/Position.h"

Scene0::Scene0(Engine* engine)
	: Scene(engine)
{
}

Scene0::~Scene0()
{
}

void Scene0::Enter(Engine* engine)
{
	using namespace GBA;
	using namespace GBA::DisplayOptions;

	int totalTestSprites = 1;

	DisplayControl::SetDisplayOptions(Mode0 | Sprites | MappingMode1D);

	SpriteLibrary* spriteLibrary = engine->EditComponent<SpriteManager>()->GetSpriteLibrary();
	ECS::EntityComponentManager* entityManager = engine->GetEntityRegistry();

	m_gameObjects.Reserve(totalTestSprites);

	SpriteAnimation idleAnim;
	{
		const int maxFrameCount = 12;
		idleAnim.keyFrames.Reserve(maxFrameCount);

		for (int i = 0; i < maxFrameCount; ++i)
		{
			Sprite* sprite = spriteLibrary->GetSprite(SpriteAtlusID::Shantae_Idle, i);
			SpriteAnimation::KeyFrame* keyframe = idleAnim.keyFrames.AddNew();
			keyframe->sprite = sprite;
		}

		idleAnim.frameRate = 12;
	}

	if (true)
	{
		for (int i = 0; i < totalTestSprites; ++i)
		{
			GameObject* testBackgroundObject = m_gameObjects.AddNew(entityManager);

			Component::Position* position = testBackgroundObject->EditComponent<Component::Position>();
			position->x = (i * 0.2f) - 5;
			position->y = (i * 0.2f) - 5;

			Component::SpriteRenderer& testBackgroundRenderer = testBackgroundObject->AddComponent<Component::SpriteRenderer>();
			Sprite* shantae0 = spriteLibrary->GetSprite(SpriteAtlusID::Shantae_Idle, 0);
			testBackgroundRenderer.SetSprite(shantae0);

			Component::SpriteAnimator& animator = testBackgroundObject->AddComponent<Component::SpriteAnimator>();
			animator.SetAnimation(idleAnim);
		}
	}

	// Entt is iterating these backwards when rendering sprites. Add player movement one last to draw on top for now. 
	{
		//GameObject* underBlack = &m_gameObjects[0];// m_gameObjects.Count() - 1];
		//Component::Position* position = underBlack->EditComponent<Component::Position>();
		//position->x = -8;
		//position->y = 0;

		GameObject* playerObject = &m_gameObjects[m_gameObjects.Count() - 1];
		Component::PlayerMovement& playerMovement = playerObject->AddComponent<Component::PlayerMovement>();
		playerMovement.moveSpeed = 8.0f;

		Component::Position* position = playerObject->EditComponent<Component::Position>();
		position->x = 0;
		position->y = 0;
	}
}

void Scene0::Update(Engine* engine)
{
	System::PlayerMovement::Update(engine);
	Scene::Update(engine);
}
