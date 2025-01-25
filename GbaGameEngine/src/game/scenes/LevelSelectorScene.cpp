#include "LevelSelectorScene.h"

#include "engine/engine/engine.h"
#include "engine/io/FileSystem.h"
#include "engine/scene/SceneManager.h"
#include "engine/graphics/GraphicsSetup.h"
#include "game/data/tilemaps/UiAtlas.h"
#include "engine/base/core/Memory.h"
#include "game/input/Input.h"

#include "game/scenes/Scene0.h"
#include "game/scenes/TilemapTestScene.h"
#include "game/scenes/CutsceneTestScene.h"

int labelXPosition = 2;
int selectedNotchXPosition = 0;

int columnStartIndex = 1;
int columnHeight = 1;

UiAtlasObject notchUiElement = UiAtlasObject::Ascii_62;
UiAtlasObject menuClearBgUiElement = UiAtlasObject::Ascii_32;

Array<std::string, 3> labels = {
	"Sprite Stress Test",
	"Background Viewer",
	"Cutscene Test",
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
	int yPos = GetSelectedLabelYPos(m_currentSelectedIndex);
	m_uiRenderCommandQueue.Enque([this, yPos] {
		m_uiRenderer.DrawUiElement(Vector2i(selectedNotchXPosition, yPos), menuClearBgUiElement);
	});
}

void LevelSelectorScene::DrawNotch()
{
	int yPos = GetSelectedLabelYPos(m_currentSelectedIndex);
	m_uiRenderCommandQueue.Enque([this, yPos] {
		m_uiRenderer.DrawUiElement(Vector2i(selectedNotchXPosition, yPos), notchUiElement);
	});
}

LevelSelectorScene::LevelSelectorScene() : Scene()
{
}

void LevelSelectorScene::Enter()
{
	GraphicsSetup::InitialiseStandardGraphics();

	IO::FileSystem* fileSystem = Engine::GetInstance().GetComponent<IO::FileSystem>();
	MemoryMappedFileView file = fileSystem->Open("tilemaps/UiAtlas");

	m_uiRenderer.LoadAtlas(file);

	Vector2i drawPosition(labelXPosition, columnStartIndex);

	for (const std::string& label : labels)
	{
		m_uiRenderCommandQueue.Enque([this, label, drawPosition] {
			m_uiRenderer.RenderText(label, drawPosition);
		});
		drawPosition.y += columnHeight;
	}

	// Draw initial notch position
	DrawNotch();
}

void LevelSelectorScene::Update()
{
	Input::InputManager* inputManager = Engine::GetInstance().GetComponent<Input::InputManager>();

	if (Input::GetInputDown(MenuDown, inputManager->GetDevices()))
	{
		AdjustSelectedIndex(1);
	}

	if (Input::GetInputDown(MenuUp, inputManager->GetDevices()))
	{
		AdjustSelectedIndex(-1);
	}

	if (Input::GetInputDown(MenuSelect, inputManager->GetDevices()))
	{
		SceneManager* sceneManager = Engine::GetInstance().GetComponent<SceneManager>();

		switch (m_currentSelectedIndex)
		{
		case 0:
		{
			sceneManager->ChangeScene<Scene0>();
			break;
		}
		case 1:
		{
			sceneManager->ChangeScene<TilemapTestScene>();
			break;
		}
		case 2:
		{
			sceneManager->ChangeScene<CutsceneTestScene>();
			break;
		}

		default:
		{
			break;
		}
		}
	}
}

void LevelSelectorScene::Render()
{
	Scene::Render();

	m_uiRenderCommandQueue.ExecuteCommands();
}
