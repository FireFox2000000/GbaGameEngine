#include "Scene0.h"
#include "engine/engine/engine.h"
#include "engine/gba/registers/display/GBADisplayControl.h"
#include "engine/render/SpriteRenderer.h"
#include "engine/graphicalassets/sprite/SpriteManager.h"
#include "engine/asset/libraries/SpriteLibrary.h"
#include "engine/asset/libraries/AnimationLibrary.h"
#include "engine/asset/libraries/FontLibrary.h"

#include "game/scripts/MovementTest.h"
#include "engine/animation/SpriteAnimator.h"
#include "engine/gameobject/transformation/Transform.h"
#include "engine/gameobject/ui/ScreenTransform.h"
#include "engine/gameobject/ui/Text.h"
#include <stdio.h>
const int totalTestSprites = 81;

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

	DisplayControl::SetDisplayOptions(Mode0 | Sprites | MappingMode1D);

	SpriteLibrary* spriteLibrary = engine->EditComponent<SpriteLibrary>();
	AnimationLibrary* animationLibrary = engine->EditComponent<AnimationLibrary>();
	FontLibrary* fontLibrary = engine->EditComponent<FontLibrary>();

	ECS::EntityComponentManager* entityManager = engine->GetEntityRegistry();

	m_gameObjects.Reserve(totalTestSprites);

	if (true)
	{
		for (int i = 0; i < 1; ++i)
		{
			GameObject* testBackgroundObject = m_gameObjects.AddNew(entityManager);

			Component::Transform* transform = testBackgroundObject->EditComponent<Component::Transform>();
			transform->position.x = (i * 0.2f) - 5;
			transform->position.y = (i * 0.2f) - 5;

			Component::SpriteRenderer& testBackgroundRenderer = testBackgroundObject->AddComponent<Component::SpriteRenderer>();
			Sprite* shantae0 = spriteLibrary->GetSprite(SpriteAtlusID::Shantae_Idle, 0);
			testBackgroundRenderer.SetSprite(shantae0);

			Component::SpriteAnimator& animator = testBackgroundObject->AddComponent<Component::SpriteAnimator>();
			animator.SetAnimation(animationLibrary->GetSpriteAnimation(SpriteAnimationID::Shantae_Idle));
		}

		{
			GameObject* testTextObject = m_gameObjects.AddNew(entityManager);
			Component::Transform* transform = testTextObject->EditComponent<Component::Transform>();
			transform->position.x = (0.2f) - 5;
			transform->position.y = (0.2f) - 5;

			Component::SpriteRenderer& testBackgroundRenderer = testTextObject->AddComponent<Component::SpriteRenderer>();
			Sprite* sprite = fontLibrary->GetFont(FontID::debug_font_8x8)->GetSpriteForCharacter('r');
			testBackgroundRenderer.SetSprite(sprite);
		}

		{
			GameObject* testTextObject = m_gameObjects.AddNew(entityManager);
			testTextObject->RemoveComponent<Component::Transform>();

			auto& screenTransform = testTextObject->AddComponent<Component::UI::ScreenTransform>();
			screenTransform.position.x = 0;
			screenTransform.position.y = 0;

			auto& textComponent = testTextObject->AddComponent<Component::UI::Text>();
			textComponent.m_font = fontLibrary->GetFont(FontID::debug_font_8x8);
			textComponent.m_str = std::string("789 ABCDEFGHIJKLMNOP\nQRSTUVWXYZ");

			this->textObject = testTextObject;
		}
	}

	// Entt is iterating these backwards when rendering sprites. Add player movement one last to draw on top for now. 
	{
		//GameObject* underBlack = &m_gameObjects[0];// m_gameObjects.Count() - 1];
		//Component::Position* position = underBlack->EditComponent<Component::Position>();
		//position->x = -8;
		//position->y = 0;

		GameObject* playerObject = &m_gameObjects[0];
		Component::PlayerMovement& playerMovement = playerObject->AddComponent<Component::PlayerMovement>();
		playerMovement.moveSpeed = 8.0f;

		Component::Transform* transform = playerObject->EditComponent<Component::Transform>();
		transform->position.x = 0;
		transform->position.y = 0;

		this->playerObject = playerObject;
	}
}

void Scene0::Update(Engine* engine)
{
	if (m_gameObjects.Count() < totalTestSprites)
	{
		int i = m_gameObjects.Count();
	
		SpriteLibrary* spriteLibrary = engine->EditComponent<SpriteLibrary>();
		AnimationLibrary* animationLibrary = engine->EditComponent<AnimationLibrary>();
	
		ECS::EntityComponentManager* entityManager = engine->GetEntityRegistry();
	
		// Create a new one
		GameObject* testBackgroundObject = m_gameObjects.AddNew(entityManager);
	
		Component::Transform* transform = testBackgroundObject->EditComponent<Component::Transform>();
		transform->position.x = (i * 0.2f) - 5;
		transform->position.y = (i * 0.2f) - 5;
	
		Component::SpriteRenderer& testBackgroundRenderer = testBackgroundObject->AddComponent<Component::SpriteRenderer>();
		Sprite* shantae0 = spriteLibrary->GetSprite(SpriteAtlusID::Shantae_Idle, 0);
		testBackgroundRenderer.SetSprite(shantae0);
	
		Component::SpriteAnimator& animator = testBackgroundObject->AddComponent<Component::SpriteAnimator>();
		animator.SetAnimation(animationLibrary->GetSpriteAnimation(SpriteAnimationID::Shantae_Idle));
	}

	//auto position = playerObject->GetComponent<Component::Transform>()->position;
	//auto* textComponent = textObject->EditComponent<Component::UI::Text>();
	//
	//char buff[100];
	//snprintf(buff, sizeof(buff), "(x = %.2f, y = %.2f)", position.x.ToFloat(), position.y.ToFloat());
	//
	//textComponent->m_str = buff;

	System::PlayerMovement::Update(engine);
	Scene::Update(engine);
}
