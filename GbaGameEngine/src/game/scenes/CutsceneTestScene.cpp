#include "CutsceneTestScene.h"
#include "engine/engine/engine.h"
#include "engine/graphics/Graphics.h"
#include "engine/graphics/GraphicsSetup.h"
#include "engine/io/FileSystem.h"
#include "engine/render/TilemapRenderer.h"
#include "engine/scene/SceneManager.h"

#include "game/scenes/LevelSelectorScene.h"
#include "game/scripts/states/BgFadeInRulestate.h"
#include "game/scripts/states/BgFadeOutRulestate.h"
#include "game/scripts/states/DialogueRulestate.h"

CutsceneTestScene::CutsceneTestScene(Engine* engine)
	: Scene(engine)
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

void CutsceneTestScene::Enter(Engine* engine)
{
	GraphicsSetup::InitialiseStandardGraphics();

	IO::FileSystem* fileSystem = engine->GetComponent<IO::FileSystem>();

	FilePtr uiAtlusFile = fileSystem->Open("tilemaps/UiAtlus");
	m_uiRenderer.LoadAtlus(uiAtlusFile);

	m_assetManager.AddTilemapSetFromFile(TilemapSetID::CutsceneImg1, fileSystem->Open("tilemaps/NightSky"));
	m_assetManager.AddTilemapSetFromFile(TilemapSetID::CutsceneImg2, fileSystem->Open("tilemaps/NightSky_Inverted"));

	Tilemap* tilemap = m_assetManager.GetTilemap(TilemapSetID::CutsceneImg1, 0);

	// Load the tilemap into vram
	Graphics* graphicsManager = engine->GetComponent<Graphics>();
	graphicsManager->LoadTilemap(*tilemap);

	Component::TilemapRenderer& tilemapRenderer = m_cutsceneBg.AddComponent<Component::TilemapRenderer>();
	tilemapRenderer.SetTilemap(tilemap);

	m_stateMachine.ChangeState<BgFadeInRulestate>([this]() {

		const char* script = "adlkjasd aslhas asbas a albasd asl as asd lasd asd lasd as dla sdl asd lasd";
		m_stateMachine.ChangeState<DialogueRulestate>(&m_uiRenderer, &m_uiRenderCommandQueue, script, 2, [this]() {
			m_stateMachine.ChangeState<BgFadeOutRulestate>([this]() {
				// Change background
				// Need to make sure was have no active gfx tasks before we do this
				ChangeBg(m_assetManager.GetTilemap(TilemapSetID::CutsceneImg2, 0));
				m_stateMachine.ChangeState<BgFadeInRulestate>([this]() { 
					const char* script = "Diag 2 adlkjasd aslhas asbas a albasd asl as asd lasd asd lasd as dla sdl asd lasd";
					m_stateMachine.ChangeState<DialogueRulestate>(&m_uiRenderer, &m_uiRenderCommandQueue, script, 2, [this]() { 
						m_stateMachine.ChangeState<BgFadeOutRulestate>([this]() { 
							SceneManager* sceneManager = Engine::GetInstance().GetComponent<SceneManager>();
							sceneManager->ChangeScene<LevelSelectorScene>(&Engine::GetInstance());
						});
					});
				});
			});
		});
	});
}

void CutsceneTestScene::Exit(Engine* engine)
{
}

void CutsceneTestScene::Update(Engine* engine)
{
	m_stateMachine.Update();
}

void CutsceneTestScene::Render(Engine* engine)
{
	Scene::Render(engine);

	m_uiRenderCommandQueue.ExecuteCommands();
}
