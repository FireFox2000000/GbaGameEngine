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

	DisplayControl::SetDisplayOptions(Mode0 | Sprites | MappingMode1D);

	SpriteLibrary* spriteLibrary = engine->GetSpriteManager()->GetSpriteLibrary();
	gameObjects.Reserve(2);
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
		GameObject* testBackgroundObject = gameObjects.AddNew();
		SpriteRenderer* testBackgroundRenderer = testBackgroundObject->AddComponent<SpriteRenderer>();

		Sprite* shantae0 = spriteLibrary->GetSprite(SpriteAtlusID::Shantae_Idle, 0);
		testBackgroundRenderer->SetSprite(shantae0);

		renderList.Add(testBackgroundRenderer);
	}
}

Scene0::~Scene0()
{
}
