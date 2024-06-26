#include "CutsceneTestScene.h"
#include "engine/engine/engine.h"
#include "engine/graphics/Graphics.h"
#include "engine/graphics/GraphicsSetup.h"
#include "engine/io/FileSystem.h"
#include "engine/render/TilemapRenderer.h"
#include "engine/scene/SceneManager.h"

#include "game/scenes/LevelSelectorScene.h"
#include "game/scripts/states/BgFadeInState.h"
#include "game/scripts/states/BgFadeOutState.h"
#include "game/scripts/states/DialogueState.h"
#include "game/scripts/states/CutsceneState.h"

#include "game/data/Tilemaps/UiAtlus.h"

const DialogueState::BorderSpriteSlice UiBorderDef = {
	UiAtlusObject::Border_TopLeft
	, UiAtlusObject::Border_TopRight
	, UiAtlusObject::Border_BottomLeft
	, UiAtlusObject::Border_BottomRight
	, UiAtlusObject::Border_Left
	, UiAtlusObject::Border_Right
	, UiAtlusObject::Border_Top
	, UiAtlusObject::Border_Bottom
};

CutsceneTestScene::CutsceneTestScene()
	: Scene()
{
}

void CutsceneTestScene::ChangeBg(Tilemap* newTilemap)
{
	auto* tilemapRenderer = m_cutsceneBg.EditComponent<Component::TilemapRenderer>();

	Tilemap* oldTilemap = tilemapRenderer->GetTilemap();

	Graphics* gfx = Engine::GetInstance().GetComponent<Graphics>();
	gfx->Unload(oldTilemap);
	gfx->LoadTilemap(*newTilemap);

	tilemapRenderer->SetTilemap(newTilemap);
}

void CutsceneTestScene::Enter()
{
	GraphicsSetup::InitialiseStandardGraphics();

	IO::FileSystem* fileSystem = Engine::GetInstance().GetComponent<IO::FileSystem>();

	FilePtr uiAtlusFile = fileSystem->Open("tilemaps/UiAtlus");
	m_uiRenderer.LoadAtlus(uiAtlusFile);

	m_assetManager.AddTilemapSetFromFile(TilemapSetID::CutsceneImg1, fileSystem->Open("tilemaps/NightSkySet"));
	m_assetManager.AddTilemapSetFromFile(TilemapSetID::CutsceneImg2, fileSystem->Open("tilemaps/NightSky_Inverted"));

	Tilemap* tilemap = m_assetManager.GetTilemap(TilemapSetID::CutsceneImg1, 0);

	// Load the tilemap into vram
	Graphics* graphicsManager = Engine::GetInstance().GetComponent<Graphics>();
	graphicsManager->LoadTilemap(*tilemap);

	Component::TilemapRenderer& tilemapRenderer = m_cutsceneBg.AddComponent<Component::TilemapRenderer>();
	tilemapRenderer.SetTilemap(tilemap);

	CutsceneState::CutsceneCommandQueue cutsceneCommands = {
		[](CutsceneState::CutsceneStateMachine* stateMachine, CutsceneState::CommandFinishedFn finished) { stateMachine->ChangeState<BgFadeInState>(finished); },
		
		[this](CutsceneState::CutsceneStateMachine* stateMachine, CutsceneState::CommandFinishedFn finished) {
			const char* script = "adlkjasd`asd";
			stateMachine->ChangeState<DialogueState>(&m_uiRenderer, &m_uiRenderCommandQueue, script, 2, UiBorderDef, finished);
		},

		[](CutsceneState::CutsceneStateMachine* stateMachine, CutsceneState::CommandFinishedFn finished) {
			stateMachine->ChangeState<BgFadeOutState>(finished);
		},

		[this](CutsceneState::CutsceneStateMachine* stateMachine, CutsceneState::CommandFinishedFn finished) {
			// Change background
			// Need to make sure was have no active gfx tasks before we do this
			ChangeBg(m_assetManager.GetTilemap(TilemapSetID::CutsceneImg2, 0));
			stateMachine->ChangeState<BgFadeInState>(finished);
		},

		[this](CutsceneState::CutsceneStateMachine* stateMachine, CutsceneState::CommandFinishedFn finished) {
			const char* script = "Diag 2 adlkjasd aslhas asbas a albasd asl as asd lasd asd lasd as dla sdl asd\nlasd";
			stateMachine->ChangeState<DialogueState>(&m_uiRenderer, &m_uiRenderCommandQueue, script, 2, UiBorderDef, finished);
		},

		[](CutsceneState::CutsceneStateMachine* stateMachine, CutsceneState::CommandFinishedFn finished) {
			stateMachine->ChangeState<BgFadeOutState>(finished);
		},
	};

	m_stateMachine.ChangeState<CutsceneState>(cutsceneCommands, [this]() {
		SceneManager* sceneManager = Engine::GetInstance().GetComponent<SceneManager>();
		sceneManager->ChangeScene<LevelSelectorScene>();
	});
}

void CutsceneTestScene::Exit()
{
	m_assetManager.Dispose(&Engine::GetInstance());
}

void CutsceneTestScene::Update()
{
	m_stateMachine.Update();
}

void CutsceneTestScene::Render()
{
	Scene::Render();

	m_uiRenderCommandQueue.ExecuteCommands();
}
