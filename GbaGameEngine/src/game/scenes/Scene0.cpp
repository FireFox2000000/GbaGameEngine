#include "Scene0.h"
#include "engine\base\colour\Colour.h"
#include "engine\gba\registers\display\GBADisplayControl.h"
#include "engine\gba\graphics\tiles\GBAPaletteBank.h"
#include "engine\gba\graphics\tiles\GBATileBank.h"
#include "engine\render\SpriteRenderer.h"
#include "engine\graphicalassets\sprite\SpriteLoader.h"

#include "game\scripts\MovementTest.h"

Scene0::Scene0()
{
	using namespace GBA;
	using namespace GBA::DisplayOptions;

	DisplayControl::SetDisplayOptions(Mode0 | Sprites | MappingMode1D);

	GameObject* testObject = gameObjects.AddNew();
	testObject->AddComponent<MovementTest>();
	SpriteRenderer* testRenderer = testObject->AddComponent<SpriteRenderer>();

	GameObject* testBackgroundObject = gameObjects.AddNew();
	SpriteRenderer* testBackgroundRenderer = testBackgroundObject->AddComponent<SpriteRenderer>();

	renderList.Add(testRenderer);
	renderList.Add(testBackgroundRenderer);

	SpriteLoader* spriteLoader = SpriteLoader::GetCurrent();
	Sprite* shantae0 = m_spriteLib.GetSprite(SpriteAtlusID::Shantae_Idle, 0);
	Sprite* shantae6 = m_spriteLib.GetSprite(SpriteAtlusID::Shantae_Idle, 6);
	spriteLoader->Load(*shantae0);
	spriteLoader->Load(*shantae6);

	testRenderer->SetSprite(shantae6);
	testBackgroundRenderer->SetSprite(shantae0);
}

Scene0::~Scene0()
{
}
