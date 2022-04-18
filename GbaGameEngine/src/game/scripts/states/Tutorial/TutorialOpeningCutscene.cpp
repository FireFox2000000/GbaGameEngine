#include "TutorialOpeningCutscene.h"
#include "engine/gameobject/transformation/Transform.h"

#include "game/scripts/states/CutsceneState.h"
#include "game/scripts/states/DialogueState.h"
#include "game/scripts/states/Tutorial/TutorialGameplayState.h"
#include "game/scripts/states/BgFadeOutState.h"
#include "game/scripts/states/BgFadeInState.h"
#include "game/scripts/states/DelayState.h"

#include "engine/render/TilemapRenderer.h"
#include "engine/graphics/Graphics.h"
#include "engine/engine/engine.h"
#include "engine/io/FileSystem.h"

#include <string>

// Should really move this def to tutorial state
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

void TutorialOpeningCutscene::ChangeBg(Tilemap* newTilemap)
{
	auto* tilemapRenderer = m_cutsceneBg->EditComponent<Component::TilemapRenderer>();

	Tilemap* oldTilemap = tilemapRenderer->GetTilemap();

	Graphics* gfx = Engine::GetInstance().GetComponent<Graphics>();
	gfx->Unload(oldTilemap);
	gfx->LoadTilemap(*newTilemap);

	tilemapRenderer->SetTilemap(newTilemap);
}

void TutorialOpeningCutscene::Enter(TutorialStateParams* params)
{
	DEBUG_LOG("Entered TutorialOpeningCutscene");

	IO::FileSystem* fileSystem = Engine::GetInstance().GetComponent<IO::FileSystem>();
	m_sagumeBg = std::make_shared<GameObject>();
	m_cutsceneBg = std::make_shared<GameObject>();
	m_tilemapContainer.AddTilemapSetFromFile(TilemapSetID::CutsceneImg1, fileSystem->Open("tilemaps/opening_cutscene_set"));

	//for (int i = TilemapSetID::Sagume1; i <= TilemapSetID::Sagume9; ++i)
	//{
	//	m_tilemapContainer.AddTilemapSetFromFile((TilemapSetID)i, fileSystem->Open((std::string("tilemaps/sagumep") + std::to_string(i)).c_str()));
	//}

	{
		Tilemap* tilemap = m_tilemapContainer.GetTilemap(TilemapSetID::CutsceneImg1, 1);

		// Load the tilemap into vram
		Graphics* graphicsManager = Engine::GetInstance().GetComponent<Graphics>();
		graphicsManager->LoadTilemap(*tilemap);

		Component::TilemapRenderer& tilemapRenderer = m_sagumeBg->AddComponent<Component::TilemapRenderer>();
		tilemapRenderer.SetTilemap(tilemap);
	}
	{
		Tilemap* tilemap = m_tilemapContainer.GetTilemap(TilemapSetID::CutsceneImg1, 0);

		// Load the tilemap into vram
		Graphics* graphicsManager = Engine::GetInstance().GetComponent<Graphics>();
		graphicsManager->LoadTilemap(*tilemap);

		Component::TilemapRenderer& tilemapRenderer = m_cutsceneBg->AddComponent<Component::TilemapRenderer>();
		tilemapRenderer.SetTilemap(tilemap);
	}

	// Hide irrelevent gameobjects
	{
		auto* transform = params->sagumeGameObject->EditComponent<Component::Transform>();
		transform->SetPosition(-99, -99);
	}
	{
		auto* transform = m_cutsceneBg->EditComponent<Component::Transform>();
		transform->SetPosition(1, -6);	
	}
	{
		auto* transform = m_sagumeBg->EditComponent<Component::Transform>();
		transform->SetPosition(1, -6);
	}

	CutsceneState::CutsceneCommandQueue cutsceneCommands = {
		//[this, params](CutsceneState::CutsceneStateMachine* stateMachine, CutsceneState::CommandFinishedFn finished) {

		//
		//	stateMachine->ChangeState<BgFadeInState>(finished);
		//},
		[this, params](CutsceneState::CutsceneStateMachine* stateMachine, CutsceneState::CommandFinishedFn finished) {
			Component::TilemapRenderer* tilemapRenderer = m_sagumeBg->EditComponent<Component::TilemapRenderer>();
			tilemapRenderer->SetVisible(false);
			
			const char* script = 
				"*Brssssshhhh!!!`"
				"???: The Lunar Goddess has left the Moon. She's isolated.`"
				"???: If we can take down the Lunar Goddess, then the Moon could fall next.`"
				"???: Yes... Let's intercept her on the way back!`"
				"*Brssssshhhh!!!`"
				;
			stateMachine->ChangeState<DialogueState>(&params->uiRenderer, &params->uiRenderCommandQueue, script, 2, UiBorderDef, finished);
		},
		//[](CutsceneState::CutsceneStateMachine* stateMachine, CutsceneState::CommandFinishedFn finished) {
		//	stateMachine->ChangeState<BgFadeOutState>(finished);
		//},
		[this](CutsceneState::CutsceneStateMachine* stateMachine, CutsceneState::CommandFinishedFn finished) {
			stateMachine->ChangeState<DelayState>(1.5f, finished);
			Component::TilemapRenderer* tilemapRenderer = m_sagumeBg->EditComponent<Component::TilemapRenderer>();
			tilemapRenderer->SetVisible(true);
		},
		//[this, params](CutsceneState::CutsceneStateMachine* stateMachine, CutsceneState::CommandFinishedFn finished) {
		//	//auto* transform = params->sagumeGameObject->EditComponent<Component::Transform>();
		//	//transform->SetPosition(0, 0);
		//	//DEBUG_LOG("Sprite positioned");
		//	
		//	stateMachine->ChangeState<BgFadeInState>(finished);
		//},
		[this, params](CutsceneState::CutsceneStateMachine* stateMachine, CutsceneState::CommandFinishedFn finished) {
			const char* script =
				"Sagume: Lady Yagakoro asked me to find her some eternity herbs...\n"
				"There should be some in this area."
				;
			stateMachine->ChangeState<DialogueState>(&params->uiRenderer, &params->uiRenderCommandQueue, script, 2, UiBorderDef, finished);
		},
		[this, params](CutsceneState::CutsceneStateMachine* stateMachine, CutsceneState::CommandFinishedFn finished) {
			Component::TilemapRenderer* tilemapRenderer = m_sagumeBg->EditComponent<Component::TilemapRenderer>();
			tilemapRenderer->SetVisible(false);
			const char* script =
				"*Brsssshhhhh!!!`"
				"???: The Moon Goddess will give us trouble, so, we'll use this to try to immobilize her!`"
				"???: ...It's perfect!"
				;
			stateMachine->ChangeState<DialogueState>(&params->uiRenderer, &params->uiRenderCommandQueue, script, 2, UiBorderDef, finished);
		},
		[this, params](CutsceneState::CutsceneStateMachine* stateMachine, CutsceneState::CommandFinishedFn finished) {
			Component::TilemapRenderer* tilemapRenderer = m_sagumeBg->EditComponent<Component::TilemapRenderer>();
			tilemapRenderer->SetVisible(true);
			const char* script =
				"*Breeennnnnn!!!`"
				"Sagume: AAAAAAAAAAHHHHHH!!!!`"
				"???: We've got her! She's nothing without her wings!`"
				;
			stateMachine->ChangeState<DialogueState>(&params->uiRenderer, &params->uiRenderCommandQueue, script, 2, UiBorderDef, finished);
		},
		[this, params](CutsceneState::CutsceneStateMachine* stateMachine, CutsceneState::CommandFinishedFn finished) {
			Component::TilemapRenderer* tilemapRenderer = m_cutsceneBg->EditComponent<Component::TilemapRenderer>();
			tilemapRenderer->SetVisible(false);
			const char* script =
				"Sagume: *Pant Pant*`"
				"Sigh...`"
				"And my past manifests into a nightmare once again...`"
				;
			stateMachine->ChangeState<DialogueState>(&params->uiRenderer, &params->uiRenderCommandQueue, script, 2, UiBorderDef, finished);
		},
	};

	m_stateMachine.ChangeState<CutsceneState>(cutsceneCommands, [this]() {
		// On finished
		m_complete = true;
	});
}

void TutorialOpeningCutscene::Update(TutorialStateParams* params)
{
	m_stateMachine.Update();

	if (m_complete)
	{
		// Go to gameplay
		//params->stateMachine.ChangeState<TutorialGameplayState>(params);
	}
}

void TutorialOpeningCutscene::Exit(TutorialStateParams* params)
{
	m_stateMachine.ChangeState(nullptr);
}
