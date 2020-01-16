#include "Dialogue_Rulestate.h"
#include "engine/engine/engine.h"
#include "game/scripts/prefabs/ui/TextPrefabFunctions.h"
#include "engine/gameobject/ui/ScreenTransform.h"
#include "engine/gba/registers/input/GBAInput.h"
#include "engine/gameobject/ui/Text.h"

Dialogue_Rulestate::Dialogue_Rulestate(const std::string& script, SharedPtr<GameRulestate> finishedState)
	: m_script(script)
	, m_finishedState(finishedState)
{

}

void Dialogue_Rulestate::Enter(GameRulestateParams & params)
{
	DEBUG_LOG("Entered rulestate [Dialogue_Rulestate]");
	m_dialogueObject = std::make_unique<GameObject>(params.engine);

	TextPrefabFunctions::MakeBasicTextObj(params.engine, *m_dialogueObject.get(), FontID::DebugFont_8x8);

	auto& screenTransform = *m_dialogueObject->EditComponent<Component::UI::ScreenTransform>();
	screenTransform.position.x = 0;
	screenTransform.position.y = 0;

	auto& text = *m_dialogueObject->EditComponent<Component::UI::Text>();
	text.m_str = std::string("Test");
}

void Dialogue_Rulestate::Update(GameRulestateParams & params)
{
	using namespace GBA;

	if (Input::GetKeyDown(Buttons::A))
	{
		params.stateMachine->ChangeState(m_finishedState, params);
	}
}
