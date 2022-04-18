#include "TutorialScene.h"
#include "engine/engine/engine.h"
#include "engine/io/FileSystem.h"
#include "engine/graphics/GraphicsSetup.h"
#include "engine/asset/AnimationFactory.h"

#include "game/scripts/states/Tutorial/TutorialOpeningCutscene.h"

TutorialScene::TutorialScene()
{
}

void TutorialScene::Enter()
{
	GraphicsSetup::InitialiseStandardGraphics();

	IO::FileSystem* fileSystem = Engine::GetInstance().GetComponent<IO::FileSystem>();

	// Load assets
	FilePtr uiAtlusFile = fileSystem->Open("tilemaps/UiAtlus");
	m_sceneParams.uiRenderer.LoadAtlus(uiAtlusFile);

	DEBUG_LOG("Loading Sagume sprite atlus");
	FilePtr sagumeSpriteSheet = fileSystem->Open("sprites/Sagume");
	m_sagumeAtlus = m_spriteAssetManager.CreateSpriteAtlusFromFile(sagumeSpriteSheet);

	SagumePrefab::InitAnimationContainer(m_sagumeAtlus, &m_sagumeAnimationContainer);

	m_sceneParams.sagumeGameObject = SagumePrefab::MakeSagumePrefab(m_sagumeAnimationContainer);

	m_sceneParams.stateMachine.ChangeState<TutorialOpeningCutscene>(&m_sceneParams);
}

void TutorialScene::Update()
{
	m_sceneParams.stateMachine.Update(&m_sceneParams);
}

void TutorialScene::Exit()
{
	m_sagumeAnimationContainer.Dispose(&Engine::GetInstance());
}

void TutorialScene::Render()
{
	Scene::Render();

	m_sceneParams.uiRenderCommandQueue.ExecuteCommands();
}
