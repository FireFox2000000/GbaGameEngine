#pragma once
#include "game/scenes/TutorialScene.h"

struct TutorialStateParams;

class TutorialGameplayState : public TutorialStateParams::TutorialStateMachine::IState
{
protected:
	void Enter(TutorialStateParams* params) override;
	void Update(TutorialStateParams* params) override;
	//void Exit(TutorialStateParams* params) override;
};
