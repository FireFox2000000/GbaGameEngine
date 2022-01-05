#pragma once

enum GameInputs
{
	SoftReset,

	MoveLeft,
	MoveRight,
	MoveUp,
	MoveDown,

	ExitTilemapTestScene,

	MenuDown,
	MenuUp,
	MenuSelect,
	MenuCancel,

	ToggleMusic,

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
