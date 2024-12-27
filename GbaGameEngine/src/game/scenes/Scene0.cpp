#include "Scene0.h"
#include <stdio.h>
#include "engine/engine/engine.h"

#include "engine/render/SpriteRenderer.h"
#include "engine/asset/AnimationFactory.h"
#include "engine/animation/SpriteAnimator.h"
#include "engine/gameobject/transformation/Transform.h"
#include "engine/physics/Collider.h"
#include "engine/physics/CollisionFunctions.h"
#include "engine/io/FileSystem.h"
#include "engine/graphics/GraphicsSetup.h"
#include "engine/physics/Rigidbody.h"
#include "engine/physics/Collision.h"
#include "engine/scene/SceneManager.h"
#include "game/scenes/LevelSelectorScene.h"

#include "engine/gameobject/ui/ScreenTransform.h"
#include "engine/gameobject/ui/Text.h"

#include "game/scripts/PlayerMovement.h"
#include "game/input/Input.h"

const int totalTestSprites = 1;

Scene0::Scene0()
	: Scene()
{
}

Scene0::~Scene0()
{
}

void Scene0::Enter()
{
	GraphicsSetup::InitialiseStandardGraphics();

	IO::FileSystem* fileSystem = Engine::GetInstance().GetComponent<IO::FileSystem>();

	auto* audioManager = Engine::GetInstance().GetComponent<AudioManager>();
	FilePtr theCrowSongFile = fileSystem->Open("audio/Seliana16");
	m_backgroundMusic = audioManager->CreateFromFile(theCrowSongFile);
	audioManager->SetChannelFlag(m_backgroundMusic, AudioChannelProperties::Loop, true);
	audioManager->SetChannelAttribute(m_backgroundMusic, AudioChannelProperties::Volume, 1.0f);
	
	//audioManager->Play(m_backgroundMusic);

	// Load assets
	DEBUG_LOG("Loading Shantae sprite atlas");
	FilePtr shantaeSpriteSheet = fileSystem->Open("sprites/Shantae_Idle_bin");
	m_shantaeAtlas = Engine::GetInstance().GetSpriteAssetManager().CreateSpriteAtlasFromFile(shantaeSpriteSheet);

	DEBUG_LOG("Loading Shantae idle animations");
	m_shantaeIdleAnim = Engine::GetInstance().GetSpriteAssetManager().CreateSpriteAnimation(AnimationFactory::CreateSpriteAtlasSequencedAnimation(m_shantaeAtlas, 0, 12, 12));

	m_shantaeIdleAnim->onNewFrameHandler = [&](int frame, ECS::Entity entity, Component::SpriteAnimator* anim)
	{
		// Last frame
		if (frame == (int)m_shantaeIdleAnim->keyFrames.Count() - 1)
		{
			auto* entityManager = Engine::GetInstance().GetEntityRegistry();
			Component::Transform* transform = entityManager->EditComponent<Component::Transform>(entity);
			auto scale = transform->GetLocalScale();
			scale.x *= -1;
			transform->SetLocalScale(scale);
		}
	};

	FontLibrary* fontLibrary = &m_fontLib;

	m_gameObjects.Reserve(totalTestSprites);

	if (true)
	{
		/*
		for (int i = 0; i < 1; ++i)
		{
			GameObject* testBackgroundObject = m_gameObjects.AddNew();

			Component::Transform* transform = testBackgroundObject->EditComponent<Component::Transform>();
			transform->SetPosition((i * 0.2f) - 5, (i * 0.2f) - 5);

			Component::SpriteRenderer& testBackgroundRenderer = testBackgroundObject->AddComponent<Component::SpriteRenderer>();
			Sprite* shantae0 = m_shantaeAtlas->GetSprite(0);
			testBackgroundRenderer.SetSprite(shantae0);

			Component::SpriteAnimator& animator = testBackgroundObject->AddComponent<Component::SpriteAnimator>();
			animator.SetAnimation(m_shantaeIdleAnim);
		}
		*/
		{
			m_textObjectCollision = m_gameObjects.AddNew();
			Component::Transform* transform = m_textObjectCollision->EditComponent<Component::Transform>();
			transform->SetPosition(0, -3);
		
			Component::SpriteRenderer& testBackgroundRenderer = m_textObjectCollision->AddComponent<Component::SpriteRenderer>();
			Sprite* sprite = fontLibrary->GetFont(FontID::debug_font_8x8_bold)->GetSpriteForCharacter('r');
			testBackgroundRenderer.SetSprite(sprite);

			Component::Collider& collider = m_textObjectCollision->AddComponent<Component::Collider>();
			//collider.SetCircle(sprite->GetSize().x);
			collider.SetAABB(
				Vector2<tFixedPoint8>(tFixedPoint8(0.5f) * -sprite->GetSize().x, tFixedPoint8(0.5f) * -sprite->GetSize().y)
				, Vector2<tFixedPoint8>(tFixedPoint8(0.5f) * sprite->GetSize().x, (tFixedPoint8(0.5f) * sprite->GetSize().y))
			);
			//collider.SetCollisionMask(BIT(1));

			collider.SetOnHitHandler([&](const Collision& coll) {
				//if (playerObject && coll.InvolvedEntity(playerObject->GetEntity()))
				//{
				//	playerObject = nullptr;
				//}
			});
		}

		{
			GameObject* testTextObject = m_gameObjects.AddNew();
			testTextObject->RemoveComponent<Component::Transform>();

			auto& screenTransform = testTextObject->AddComponent<Component::UI::ScreenTransform>();
			screenTransform.position.x = 0;
			screenTransform.position.y = 0;

			auto& textComponent = testTextObject->AddComponent<Component::UI::Text>();
			textComponent.m_font = fontLibrary->GetFont(FontID::debug_font_8x8_bold);
			textComponent.m_str = std::string("789 ABCDEFGHIJKLMNOP\nQRSTUVWXYZ");

			m_textObject = testTextObject;
		}
	}

	// Entt is iterating these backwards when rendering sprites. Add player movement one last to draw on top for now. 
	{
		//GameObject* underBlack = &m_gameObjects[0];// m_gameObjects.Count() - 1];
		//Component::Position* position = underBlack->EditComponent<Component::Position>();
		//position->x = -8;
		//position->y = 0;

		Component::SpriteRenderer& spriteRenderer = m_playerObject.AddComponent<Component::SpriteRenderer>();
		Sprite* shantae0 = m_shantaeAtlas->GetSprite(1);
		spriteRenderer.SetSprite(shantae0);

		Component::SpriteAnimator& animator = m_playerObject.AddComponent<Component::SpriteAnimator>();
		animator.SetAnimation(m_shantaeIdleAnim);

		Component::Rigidbody& rigidbody = m_playerObject.AddComponent<Component::Rigidbody>();
		rigidbody.gravity = Vector2<tFixedPoint24>(0, -30);

		Component::PlayerMovement& playerMovement = m_playerObject.AddComponent<Component::PlayerMovement>();
		playerMovement.moveSpeed = 8.0f;
		playerMovement.jumpInitVel = 22.0f;

		Component::Transform* transform = m_playerObject.EditComponent<Component::Transform>();
		transform->SetPosition(0, 5);
		//transform->SetScale(1, 1);
		//transform->SetRotationDegrees(180);

		Component::Collider& collider = m_playerObject.AddComponent<Component::Collider>();
		//collider.SetCircle(tFixedPoint8(0.5f) * shantae0->GetSize().x);
		tFixedPoint8 colliderWidth = tFixedPoint8(shantae0->GetSize().x) - tFixedPoint8(1);
		collider.SetAABB(
			Vector2<tFixedPoint8>((tFixedPoint8(-0.5f) * colliderWidth) + tFixedPoint8(0.5f), tFixedPoint8(0.5f) * -shantae0->GetSize().y)
			, Vector2<tFixedPoint8>((tFixedPoint8(0.5f) * colliderWidth), (tFixedPoint8(0.5f) * shantae0->GetSize().y) - tFixedPoint8(4))
				);
		//collider.SetCollisionMask(BIT(0));
	}
}

void Scene0::Update()
{
	if (m_gameObjects.Count() < totalTestSprites)
	{
		int i = m_gameObjects.Count();
	
		// Create a new one
		GameObject* testBackgroundObject = m_gameObjects.AddNew();
	
		Component::Transform* transform = testBackgroundObject->EditComponent<Component::Transform>();
		transform->SetPosition((i * 0.2f) - 5, (i * 0.2f) - 5);
	
		Component::SpriteRenderer& testBackgroundRenderer = testBackgroundObject->AddComponent<Component::SpriteRenderer>();
		Sprite* shantae0 = m_shantaeAtlas->GetSprite(0);
		testBackgroundRenderer.SetSprite(shantae0);
	
		Component::SpriteAnimator& animator = testBackgroundObject->AddComponent<Component::SpriteAnimator>();
		animator.SetAnimation(m_shantaeIdleAnim);
	}

	if (m_textObject && true)
	{
		const auto* playerTransform = m_playerObject.GetComponent<Component::Transform>();
		auto* textComponent = m_textObject->EditComponent<Component::UI::Text>();

		{
			const Component::Collider* playerCollider = m_playerObject.GetComponent<Component::Collider>();

			const auto* letterTransform = m_textObjectCollision->GetComponent<Component::Transform>();
			const Component::Collider* letterCollider = m_textObjectCollision->GetComponent<Component::Collider>();

			Collision collision;
			if (CollisionFunctions::HasCollision(*playerTransform, *playerCollider, *letterTransform, *letterCollider, &collision))
			{
				char buff[100];
				snprintf(buff, sizeof(buff), "(Normal (%.2f, %.2f)", collision.normal.x.ToFloat(), collision.normal.y.ToFloat());

				textComponent->m_str = buff;
			}
			else
			{
				auto position = playerTransform->GetPosition();
				
				char buff[100];
				snprintf(buff, sizeof(buff), "(x = %.2f, y = %.2f)", position.x.ToFloat(), position.y.ToFloat());
				
				textComponent->m_str = buff;
			}
		}
	}

	PlayerMovement::MoveHumanPlayerObject(m_playerObject);

	Input::InputManager* inputManager = Engine::GetInstance().GetComponent<Input::InputManager>();
	const auto& devices = inputManager->GetDevices();

	if (Input::GetInputDown(ToggleMusic, devices))
	{
		auto* audioManager = Engine::GetInstance().GetComponent<AudioManager>();
		if (audioManager->GetChannelFlag(m_backgroundMusic, AudioChannelProperties::Active))
		{
			audioManager->Stop(m_backgroundMusic);
		}
		else
		{
			audioManager->Play(m_backgroundMusic);
		}
	}

	if (Input::GetInputDown(ReturnToLevelSelector, devices))
	{
		SceneManager* sceneManager = Engine::GetInstance().GetComponent<SceneManager>();
		sceneManager->ChangeScene<LevelSelectorScene>();
	}
}

void Scene0::Exit()
{
	// TODO, should add an auto dispose list to handle this kind of thing instead
	auto* audioManager = Engine::GetInstance().GetComponent<AudioManager>();
	audioManager->Stop(m_backgroundMusic);
	audioManager->FreeChannel(m_backgroundMusic);

	Engine::GetInstance().GetSpriteAssetManager().UnloadSpriteAnimation(m_shantaeIdleAnim);
	Engine::GetInstance().GetSpriteAssetManager().UnloadSpriteAtlas(m_shantaeAtlas);

	m_assetManager.Dispose(&Engine::GetInstance());
}
