#include "Dialogue_Rulestate.h"

Dialogue_Rulestate::Dialogue_Rulestate(const std::string& script, SharedPtr<GameRulestate> finishedState)
	: m_script(script)
	, m_finishedState(finishedState)
{

}

void Dialogue_Rulestate::Enter(GameRulestateParams & params)
{
	DEBUG_LOG("Entered rulestate [Dialogue_Rulestate]");
}

void Dialogue_Rulestate::Update(GameRulestateParams & params)
{
	params.stateMachine->ChangeState(m_finishedState, params);
}
