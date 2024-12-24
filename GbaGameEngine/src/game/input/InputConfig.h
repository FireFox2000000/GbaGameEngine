#pragma once

enum GameInputs
{
	SoftReset,

	MoveLeft,
	MoveRight,
	MoveUp,
	MoveDown,

	Jump,

	ExitTilemapTestScene,
	ReturnToLevelSelector,

	MenuDown,
	MenuUp,
	MenuSelect,
	MenuCancel,

	ToggleMusic,

	AdvanceDialogue,
	SkipDialogue,

	Count
};

namespace Input
{
	struct InputMapIter;
}

namespace InputConfig
{
	Input::InputMapIter GetGameInputIter(GameInputs input, int mapForDeviceId);
}
