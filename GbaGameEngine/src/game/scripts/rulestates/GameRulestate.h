#pragma once
#include "engine/util/state/StateMachine.h"

struct GameRulestateParams;

using GameStateMachine = StateMachine<GameRulestateParams&>;
using GameRulestate = GameStateMachine::IState;

class Engine;
class GameObject;

struct GameRulestateParams
{
	Engine* engine;
	GameStateMachine* stateMachine;
	GameObject* playerObject;
};

