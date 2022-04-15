#include "TutorialScene.h"
#include "engine/engine/engine.h"
#include "engine/io/FileSystem.h"
#include "engine/graphics/GraphicsSetup.h"
#include "engine/asset/AnimationFactory.h"

TutorialScene::TutorialScene()
{
}

void TutorialScene::Enter()
{
	GraphicsSetup::InitialiseStandardGraphics();

	IO::FileSystem* fileSystem = Engine::GetInstance().GetComponent<IO::FileSystem>();

	// Load assets
	DEBUG_LOG("Loading Sagume sprite atlus");
	FilePtr sagumeSpriteSheet = fileSystem->Open("sprites/Sagume");
	m_sagumeAtlus = m_spriteAssetManager.CreateSpriteAtlusFromFile(sagumeSpriteSheet);

	DEBUG_LOG("Loading Sagume animations");
	SagumePrefab::InitAnimationContainer(m_sagumeAtlus, &m_sagumeAnimationContainer);

	m_sagumeGameObject = SagumePrefab::MakeSagumePrefab(m_sagumeAnimationContainer);
}

void TutorialScene::Update()
{
}

void TutorialScene::Exit()
{
	m_sagumeAnimationContainer.Dispose(&Engine::GetInstance());
}
