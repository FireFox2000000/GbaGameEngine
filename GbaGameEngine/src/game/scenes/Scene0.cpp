#include "Scene0.h"
#include "engine/base/colour/Colour.h"
#include "engine/engine/engine.h"
#include "engine/gba/registers/display/GBADisplayControl.h"
#include "engine/gba/graphics/tiles/GBAPaletteBank.h"
#include "engine/gba/graphics/tiles/GBATileBank.h"
#include "engine/render/SpriteRenderer.h"
#include "engine/graphicalassets/sprite/SpriteManager.h"
#include "engine/graphicalassets/sprite/SpriteLibrary.h"

#include "game/scripts/MovementTest.h"

Scene0::Scene0(Engine* engine)
	: Scene(engine)
{
	using namespace GBA;
	using namespace GBA::DisplayOptions;

	DisplayControl::SetDisplayOptions(Mode0 | Sprites | MappingMode1D);

	SpriteLibrary* spriteLibrary = engine->GetSpriteManager()->GetSpriteLibrary();

	{
		GameObject* testObject = gameObjects.AddNew();
		testObject->AddComponent<MovementTest>();
		SpriteRenderer* testRenderer = testObject->AddComponent<SpriteRenderer>();

		Sprite* shantae6 = spriteLibrary->GetSprite(SpriteAtlusID::Shantae_Idle, 6);
		testRenderer->SetSprite(shantae6);
		renderList.Add(testRenderer);
	}

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
