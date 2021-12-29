#include "LevelSelectorScene.h"

#include "engine/engine/engine.h"
#include "engine/io/FileSystem.h"
#include "engine/scene/SceneManager.h"
#include "engine/gba/registers/input/GBAInput.h"
#include "engine/graphics/GraphicsSetup.h"
#include "game/data/tilemaps/UiAtlus.h"
#include "game/scenes/Scene0.h"
#include "game/scenes/TilemapTestScene.h"
#include "engine/base/core/Memory.h"

int labelXPosition = 2;
int selectedNotchXPosition = 0;

int columnStartIndex = 1;
int columnHeight = 1;

UiAtlusObject notchUiElement = UiAtlusObject::Ascii_62;
UiAtlusObject menuClearBgUiElement = UiAtlusObject::Ascii_32;

Array<std::string, 2> labels = {
	"Sprite Stress Test",
	"Background Viewer",
};

int GetSelectedLabelYPos(int currentSelectedOption)
{
	return columnStartIndex + currentSelectedOption * columnHeight;
}

void LevelSelectorScene::AdjustSelectedIndex(int direction)
{
	// Clear the previous region
	ClearNotch();

	m_currentSelectedIndex += direction;	// negative as we draw the screen with y = 0 at the top
	m_currentSelectedIndex = Math::Mod(m_currentSelectedIndex, labels.Count());

	// Highlight the new region
	DrawNotch();
}

void LevelSelectorScene::ClearNotch()
{
	m_uiRenderer.DrawUiElement(Vector2<int>(selectedNotchXPosition, GetSelectedLabelYPos(m_currentSelectedIndex)), menuClearBgUiElement);
}

void LevelSelectorScene::DrawNotch()
{
	m_uiRenderer.DrawUiElement(Vector2<int>(selectedNotchXPosition, GetSelectedLabelYPos(m_currentSelectedIndex)), notchUiElement);
}

LevelSelectorScene::LevelSelectorScene(Engine* engine) : Scene(engine)
{
}

void LevelSelectorScene::Enter(Engine* engine)
{
	GraphicsSetup::InitialiseStandardGraphics();

	IO::FileSystem* fileSystem = engine->GetComponent<IO::FileSystem>();
	FilePtr file = fileSystem->Open("tilemaps/UiAtlus");

	m_uiRenderer.LoadAtlus(file);

	Vector2<int> drawPosition(labelXPosition, columnStartIndex);

	for (const std::string& label : labels)
	{
		m_uiRenderer.RenderText(label, drawPosition);
		drawPosition.y += columnHeight;
	}

	// Draw initial notch position
	DrawNotch();
}

void LevelSelectorScene::Update(Engine* engine)
{
	if (GBA::Input::GetKeyDown(GBA::Buttons::Down))
	{
		AdjustSelectedIndex(1);
	}

	if (GBA::Input::GetKeyDown(GBA::Buttons::Up))
	{
		AdjustSelectedIndex(-1);
	}

	if (GBA::Input::GetKeyDown(GBA::Buttons::A))
	{
		SceneManager* sceneManager = engine->GetComponent<SceneManager>();

		switch (m_currentSelectedIndex)
		{
		case 0:
		{
			sceneManager->ChangeScene<Scene0>(engine);
			break;
		}
		case 1:
		{
			sceneManager->ChangeScene<TilemapTestScene>(engine);
			break;
		}

		default:
		{
			break;
		}
		}
	}
}
