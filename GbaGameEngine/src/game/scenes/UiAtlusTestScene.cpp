#include "UiAtlusTestScene.h"
#include "engine/engine/engine.h"
#include "engine/gba/registers/display/GBADisplayControl.h"
#include "game/data/tilemaps/UiAtlus.h"

UiAtlusTestScene::UiAtlusTestScene(Engine* engine) : Scene(engine)
{
}

void UiAtlusTestScene::Enter(Engine* engine)
{
	using namespace GBA::DisplayOptions;
	using namespace GBA;

	GBA::DisplayControl::SetDisplayOptions(Mode0 | Sprites | MappingMode1D);

	m_uiRenderer.LoadAtlus(UiAtlus::data);

	// Draw test on screen
	{
		std::string testString = "Hello World!";		
		const Vector2<int> drawPosition(1, 1);

		m_uiRenderer.RenderText(testString, drawPosition);
	}
}

void UiAtlusTestScene::Update(Engine* engine)
{
}
