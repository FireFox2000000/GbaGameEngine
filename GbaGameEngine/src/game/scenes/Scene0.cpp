#include "Scene0.h"
#include "engine/engine/engine.h"
#include "engine/gba/registers/display/GBADisplayControl.h"
#include "game/scripts/rulestates/GeneralGameplay_Rulestate.h"
#include "engine/base/core/stl/SharedPtr.h"

const int totalTestSprites = 90;

Scene0::Scene0(Engine* engine)
	: Scene(engine)
{
}

Scene0::~Scene0()
{
}

void Scene0::Enter(Engine* engine)
{
	DEBUG_LOG("Entered scene [Scene0]");

	using namespace GBA;
	using namespace GBA::DisplayOptions;

	DisplayControl::SetDisplayOptions(Mode0 | Sprites | MappingMode1D);

	GameRulestateParams updateParams;
	updateParams.engine = engine;
	updateParams.stateMachine = &m_rulestateMachine;

	SharedPtr<GameRulestate> nextState = std::make_shared<GeneralGameplay_Rulestate>();

	m_rulestateMachine.ChangeState(nextState, updateParams);
}

void Scene0::Update(Engine* engine)
{
	GameRulestateParams updateParams;
	updateParams.engine = engine;
	updateParams.stateMachine = &m_rulestateMachine;

	m_rulestateMachine.Update(updateParams);

	Scene::Update(engine);
}
