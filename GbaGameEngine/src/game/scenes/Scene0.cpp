#include "Scene0.h"
#include "engine/engine/engine.h"
#include "engine/gba/registers/display/GBADisplayControl.h"
#include "engine/render/SpriteRenderer.h"
#include "engine/graphicalassets/sprite/SpriteManager.h"
#include "engine/graphicalassets/sprite/SpriteLibrary.h"

#include "game/scripts/MovementTest.h"
#include "game/scripts/AnimationTest.h"

Scene0::Scene0(Engine* engine)
	: Scene(engine)
{
	using namespace GBA;
	using namespace GBA::DisplayOptions;

	int totalTestSprites = 41;

	DisplayControl::SetDisplayOptions(Mode0 | Sprites | MappingMode1D);

	SpriteLibrary* spriteLibrary = engine->GetSpriteManager()->GetSpriteLibrary();
	gameObjects.Reserve(totalTestSprites);
	{		
		GameObject* testObject = gameObjects.AddNew();

		testObject->AddComponent<MovementTest>();
		SpriteRenderer* testRenderer = gameObjects[0].AddComponent<SpriteRenderer>();
		renderList.Add(testRenderer);

		testRenderer->SetSprite(spriteLibrary->GetSprite(SpriteAtlusID::Shantae_Idle, 6));
		

		AnimationTest* animation = testObject->AddComponent<AnimationTest>();
		animation->Init(engine);
		
	}
	
	if (false)
	{
		for (int i = 1; i < totalTestSprites; ++i)
		{
			GameObject* testBackgroundObject = gameObjects.AddNew();
			SpriteRenderer* testBackgroundRenderer = testBackgroundObject->AddComponent<SpriteRenderer>();
			testBackgroundObject->SetPosition(Vector2f((i * 0.2f) - 5, (i * 0.2f) - 5));

			Sprite* shantae0 = spriteLibrary->GetSprite(SpriteAtlusID::Shantae_Idle, 0);
			testBackgroundRenderer->SetSprite(shantae0);

			AnimationTest* animation = testBackgroundObject->AddComponent<AnimationTest>();
			animation->Init(engine);

			renderList.Add(testBackgroundRenderer);
		}
	}
}

Scene0::~Scene0()
{
}
