#include "Dialogue_Rulestate.h"
#include "engine/engine/engine.h"
#include "game/scripts/prefabs/ui/TextPrefabFunctions.h"
#include "engine/gameobject/ui/ScreenTransform.h"
#include "engine/gba/registers/input/GBAInput.h"
#include "engine/gameobject/ui/Text.h"
#include "engine/screen/Screen.h"

Dialogue_Rulestate::Dialogue_Rulestate(const std::string& script, SharedPtr<GameRulestate> finishedState)
	: m_script(script)
	, m_finishedState(finishedState)
{

}

bool Dialogue_Rulestate::AdvanceText()
{
	// Find the next set of text to display in a single box
	int dialogueIndexEnd = m_currentTextIndex;
	for (; dialogueIndexEnd < (int)m_script.length(); ++dialogueIndexEnd)
	{
		if (m_script[dialogueIndexEnd] == c_dialogueBoxStepFlag)
		{
			break;
		}
	}

	if (m_currentTextIndex == dialogueIndexEnd)
	{
		// Finished
		return true;
	}

	auto& text = *m_dialogueObject->EditComponent<Component::UI::Text>();
	text.m_str = m_script.substr(m_currentTextIndex, dialogueIndexEnd - m_currentTextIndex);

	m_currentTextIndex = dialogueIndexEnd + 1;

	return false;
}

void Dialogue_Rulestate::Enter(GameRulestateParams & params)
{
	DEBUG_LOG("Entered rulestate [Dialogue_Rulestate]");
	m_dialogueObject = std::make_unique<GameObject>(params.engine);

	TextPrefabFunctions::MakeBasicTextObj(params.engine, *m_dialogueObject.get(), FontID::DebugFont_8x8);

	Vector2<int> resolution = Screen::GetResolution();

	auto& screenTransform = *m_dialogueObject->EditComponent<Component::UI::ScreenTransform>();
	screenTransform.position.x = 4;
	screenTransform.position.y = resolution.y - 16;

	if (AdvanceText())
		params.stateMachine->ChangeState(m_finishedState, params);
}

void Dialogue_Rulestate::Update(GameRulestateParams & params)
{
	using namespace GBA;

	if (Input::GetKeyDown(Buttons::A))
	{
		if (AdvanceText())
			params.stateMachine->ChangeState(m_finishedState, params);
	}
}
