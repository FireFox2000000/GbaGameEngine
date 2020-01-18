#include "IntroductionScene.h"
#include "engine/engine/engine.h"
#include "engine/gba/registers/display/GBADisplayControl.h"
#include "game/scripts/rulestates/Dialogue_Rulestate.h"
#include "engine/scene/SceneManager.h"
#include "game/scenes/Scene0.h"

IntroductionScene::IntroductionScene(Engine * engine) : Scene(engine)
{
}

IntroductionScene::~IntroductionScene()
{
}

void IntroductionScene::Enter(Engine * engine)
{
	DEBUG_LOG("Entered scene [IntroductionScene]");

	using namespace GBA;
	using namespace GBA::DisplayOptions;

	DisplayControl::SetDisplayOptions(Mode0 | Sprites | MappingMode1D);

	GameRulestateParams updateParams;
	updateParams.engine = engine;
	updateParams.stateMachine = &m_rulestateMachine;

	std::string script = "Here begins Reimu's\ntest to she if she is worthy";
	script += Dialogue_Rulestate::c_dialogueBoxStepFlag;
	script += "of the legendary lost\npurification artifact";
	script += Dialogue_Rulestate::c_dialogueBoxStepFlag;
	script += "Lost in the forest of the\nsoulless, Reimu must find a\nway to escape...";

	m_rulestateMachine.ChangeState<Dialogue_Rulestate>(updateParams, script, nullptr);
}

void IntroductionScene::Update(Engine * engine)
{
	GameRulestateParams updateParams;
	updateParams.engine = engine;
	updateParams.stateMachine = &m_rulestateMachine;

	m_rulestateMachine.Update(updateParams);

	Scene::Update(engine);

	if (!m_rulestateMachine.HasState())
	{
		engine->EditComponent<SceneManager>()->ChangeScene<Scene0>(engine);
	}
}
