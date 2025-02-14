#include "CutsceneTestScene.h"
#include "engine/engine/engine.h"
#include "engine/graphics/Graphics.h"
#include "engine/graphics/GraphicsSetup.h"
#include "engine/io/FileSystem.h"
#include "engine/graphics/tilemap/TilemapRenderer.h"
#include "engine/scene/SceneManager.h"

#include "game/scenes/LevelSelectorScene.h"
#include "game/scripts/states/BgFadeInState.h"
#include "game/scripts/states/BgFadeOutState.h"
#include "game/scripts/states/DialogueState.h"
#include "game/scripts/states/CutsceneState.h"

#include "game/data/Tilemaps/UiAtlas.h"

const DialogueState::BorderSpriteSlice UiBorderDef = {
	UiAtlasObject::Border_TopLeft
	, UiAtlasObject::Border_TopRight
	, UiAtlasObject::Border_BottomLeft
	, UiAtlasObject::Border_BottomRight
	, UiAtlasObject::Border_Left
	, UiAtlasObject::Border_Right
	, UiAtlasObject::Border_Top
	, UiAtlasObject::Border_Bottom
};

CutsceneTestScene::CutsceneTestScene()
	: Scene()
{
}

void CutsceneTestScene::ChangeBg(GBA::Gfx::Tilemap* newTilemap)
{
	auto* tilemapRenderer = m_cutsceneBg.EditComponent<TilemapRenderer>();

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
	ResourcesManager& resourcesManager = Engine::GetInstance().GetResourcesManager();

	MemoryMappedFileView uiAtlasFile = fileSystem->Open("tilemaps/UiAtlas");
	m_uiRenderer.LoadAtlas(uiAtlasFile);

	m_cutsceneImg1 = resourcesManager.LoadTilemapSet(fileSystem->Open("tilemaps/NightSkySet"));
	m_cutsceneImg2 = resourcesManager.LoadTilemapSet(fileSystem->Open("tilemaps/NightSky_Inverted"));

	Tilemap* tilemap = m_cutsceneImg1->GetTilemap(0);

	// Load the tilemap into vram
	Graphics* graphicsManager = Engine::GetInstance().GetComponent<Graphics>();
	graphicsManager->LoadTilemap(*tilemap);

	TilemapRenderer& tilemapRenderer = m_cutsceneBg.AddComponent<TilemapRenderer>();
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
			ChangeBg(m_cutsceneImg2->GetTilemap(0));
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
	ResourcesManager& resourcesManager = Engine::GetInstance().GetResourcesManager();
	resourcesManager.Unload(m_cutsceneImg1);
	resourcesManager.Unload(m_cutsceneImg2);
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
