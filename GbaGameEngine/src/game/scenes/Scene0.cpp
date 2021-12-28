#include "Scene0.h"
#include <stdio.h>
#include "engine/engine/engine.h"
#include "engine/gba/registers/display/GBADisplayControl.h"

#include "engine/render/SpriteRenderer.h"
#include "engine/asset/AnimationFactory.h"
#include "engine/animation/SpriteAnimator.h"
#include "engine/gameobject/transformation/Transform.h"
#include "engine/physics/Collider.h"
#include "engine/physics/CollisionFunctions.h"
#include "engine/io/FileSystem.h"

#include "engine/gameobject/ui/ScreenTransform.h"
#include "engine/gameobject/ui/Text.h"

#include "game/scripts/PlayerMovement.h"
#include "engine/gba/registers/input/GBAInput.h"

const int totalTestSprites = 90;

Scene0::Scene0(Engine* engine)
	: Scene(engine)
	, playerObject()
{
}

Scene0::~Scene0()
{
}

void Scene0::Enter(Engine* engine)
{
	IO::FileSystem* fileSystem = engine->GetComponent<IO::FileSystem>();

	auto* audioManager = engine->GetComponent<AudioManager>();
	FilePtr theCrowSongFile = fileSystem->Open("audio/TheCrowSong");
	m_backgroundMusic = audioManager->CreateFromFile(theCrowSongFile);
	audioManager->SetChannelFlag(m_backgroundMusic, AudioChannelProperties::Loop, true);
	audioManager->SetChannelAttribute(m_backgroundMusic, AudioChannelProperties::Volume, 0.5f);

	audioManager->Play(m_backgroundMusic);

	// Load assets
	DEBUG_LOG("Loading Shantae sprite atlus");
	FilePtr shantaeSpriteSheet = fileSystem->Open("sprites/Shantae_Idle_bin");
	shantaeAtlus = m_spriteAssetManager.CreateSpriteAtlusFromFile(shantaeSpriteSheet);

	DEBUG_LOG("Loading Shantae idle animations");
	auto* defaultIdleAnim = m_assetManager.AddSpriteAnimation(SpriteAnimationID::Shantae_Idle, AnimationFactory::CreateSpriteAtlusSequencedAnimation(shantaeAtlus, 0, 12, 12));

	{
		using namespace GBA::DisplayOptions;
		GBA::DisplayControl::SetDisplayOptions(Mode0 | Sprites | MappingMode1D);
	}

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
			Sprite* shantae0 = m_assetManager.GetSprite(SpriteAtlusID::Shantae, 0);
			testBackgroundRenderer.SetSprite(shantae0);

			Component::SpriteAnimator& animator = testBackgroundObject->AddComponent<Component::SpriteAnimator>();
			animator.SetAnimation(defaultIdleAnim);
		}
		*/
		{
			textObjectCollision = m_gameObjects.AddNew();
			Component::Transform* transform = textObjectCollision->EditComponent<Component::Transform>();
			transform->SetPosition(0, 0);
		
			Component::SpriteRenderer& testBackgroundRenderer = textObjectCollision->AddComponent<Component::SpriteRenderer>();
			Sprite* sprite = fontLibrary->GetFont(FontID::debug_font_8x8_bold)->GetSpriteForCharacter('r');
			testBackgroundRenderer.SetSprite(sprite);

			Component::Collider& collider = textObjectCollision->AddComponent<Component::Collider>();
			//collider.SetCircle(sprite->GetSize().x);
			collider.SetAABB(
				Vector2<tFixedPoint8>(tFixedPoint8(0.5f) * -sprite->GetSize().x, tFixedPoint8(0.5f) * -sprite->GetSize().y)
				, Vector2<tFixedPoint8>(tFixedPoint8(0.5f) * sprite->GetSize().x, (tFixedPoint8(0.5f) * sprite->GetSize().y))
			);
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

			this->textObject = testTextObject;
		}
	}

	// Entt is iterating these backwards when rendering sprites. Add player movement one last to draw on top for now. 
	{
		//GameObject* underBlack = &m_gameObjects[0];// m_gameObjects.Count() - 1];
		//Component::Position* position = underBlack->EditComponent<Component::Position>();
		//position->x = -8;
		//position->y = 0;

		Component::SpriteRenderer& spriteRenderer = playerObject.AddComponent<Component::SpriteRenderer>();
		Sprite* shantae0 = shantaeAtlus->GetSprite(1);
		spriteRenderer.SetSprite(shantae0);

		Component::SpriteAnimator& animator = playerObject.AddComponent<Component::SpriteAnimator>();
		animator.SetAnimation(defaultIdleAnim);

		Component::PlayerMovement& playerMovement = playerObject.AddComponent<Component::PlayerMovement>();
		playerMovement.moveSpeed = 8.0f;

		Component::Transform* transform = playerObject.EditComponent<Component::Transform>();
		transform->SetPosition(-5, 0);
		//transform->SetScale(1, 1);
		//transform->SetRotationDegrees(180);

		Component::Collider& collider = playerObject.AddComponent<Component::Collider>();
		//collider.SetCircle(tFixedPoint8(0.5f) * shantae0->GetSize().x);
		tFixedPoint8 colliderWidth = tFixedPoint8(shantae0->GetSize().x) - tFixedPoint8(1);
		collider.SetAABB(
			Vector2<tFixedPoint8>((tFixedPoint8(-0.5f) * colliderWidth) + tFixedPoint8(0.5f), tFixedPoint8(0.5f) * -shantae0->GetSize().y)
			, Vector2<tFixedPoint8>((tFixedPoint8(0.5f) * colliderWidth), (tFixedPoint8(0.5f) * shantae0->GetSize().y) - tFixedPoint8(4))
				);
	}
}

void Scene0::Update(Engine* engine)
{
	if (m_gameObjects.Count() < totalTestSprites)
	{
		int i = m_gameObjects.Count();
	
		// Create a new one
		GameObject* testBackgroundObject = m_gameObjects.AddNew();
	
		Component::Transform* transform = testBackgroundObject->EditComponent<Component::Transform>();
		transform->SetPosition((i * 0.2f) - 5, (i * 0.2f) - 5);
	
		Component::SpriteRenderer& testBackgroundRenderer = testBackgroundObject->AddComponent<Component::SpriteRenderer>();
		Sprite* shantae0 = shantaeAtlus->GetSprite(0);
		testBackgroundRenderer.SetSprite(shantae0);
	
		Component::SpriteAnimator& animator = testBackgroundObject->AddComponent<Component::SpriteAnimator>();
		animator.SetAnimation(m_assetManager.GetAsset(SpriteAnimationID::Shantae_Idle));
	}

	if (textObject && false)
	{
		const auto* playerTransform = playerObject.GetComponent<Component::Transform>();
		auto* textComponent = textObject->EditComponent<Component::UI::Text>();

		{
			const Component::Collider* playerCollider = playerObject.GetComponent<Component::Collider>();

			const auto* letterTransform = textObjectCollision->GetComponent<Component::Transform>();
			const Component::Collider* letterCollider = textObjectCollision->GetComponent<Component::Collider>();

			if (CollisionFunctions::HasCollision(*playerTransform, *playerCollider, *letterTransform, *letterCollider))
			{
				textComponent->m_str = "Collision";
			}
			else
			{
				//textComponent->m_str = "Nollision";
				//auto position = playerTransform->GetPosition();
				//
				//char buff[100];
				//snprintf(buff, sizeof(buff), "(x = %.2f, y = %.2f)", position.x.ToFloat(), position.y.ToFloat());
				//
				//textComponent->m_str = buff;
			}
		}
	}

	PlayerMovement::MoveHumanPlayerObject(engine, playerObject);

	if (GBA::Input::GetKeyDown(GBA::Buttons::R))
	{
		auto* audioManager = engine->GetComponent<AudioManager>();
		if (audioManager->GetChannelFlag(m_backgroundMusic, AudioChannelProperties::Active))
		{
			audioManager->Stop(m_backgroundMusic);
		}
		else
		{
			audioManager->Play(m_backgroundMusic);
		}
	}
}

void Scene0::Exit(Engine * engine)
{
	// TODO, should add an auto dispose list to handle this kind of thing instead
	auto* audioManager = engine->GetComponent<AudioManager>();
	audioManager->Stop(m_backgroundMusic);
	audioManager->FreeChannel(m_backgroundMusic);

	m_assetManager.Dispose(engine);
}
