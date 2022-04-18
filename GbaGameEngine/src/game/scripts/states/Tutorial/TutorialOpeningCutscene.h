#pragma once
#include "game/scenes/TutorialScene.h"

struct TutorialStateParams;

class TutorialOpeningCutscene : public TutorialStateParams::TutorialStateMachine::IState
{
	StateMachine<> m_stateMachine;
	bool m_complete = false;
	SharedPtr<GameObject> m_cutsceneBg;
	SharedPtr<GameObject> m_sagumeBg;

	enum SpriteAnimationID
	{
		AnimationCount
	};

	enum TilemapSetID {
		CutsceneImg1,
		Sagume1,
		Sagume2,
		Sagume3,
		Sagume4,
		Sagume5,
		Sagume6,
		Sagume7,
		Sagume8,
		Sagume9,

		TilemapSetCount
	};

	FixedAssetManager<
		SpriteAnimationID,
		TilemapSetID>
		m_tilemapContainer;

	void ChangeBg(Tilemap* newTilemap);

protected:
	void Enter(TutorialStateParams* params) override;
	void Update(TutorialStateParams* params) override;
	void Exit(TutorialStateParams* params) override;
};
