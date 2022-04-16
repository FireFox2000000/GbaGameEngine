#pragma once
#include "game/scenes/TutorialScene.h"

struct TutorialStateParams;

class TutorialOpeningCutscene : public TutorialStateParams::TutorialStateMachine::IState
{
	StateMachine<> m_stateMachine;
	bool m_complete = false;
	SharedPtr<GameObject> m_cutsceneBg;

	void ChangeBg(Tilemap* newTilemap);

protected:
	void Enter(TutorialStateParams* params) override;
	void Update(TutorialStateParams* params) override;
	void Exit(TutorialStateParams* params) override;
};
