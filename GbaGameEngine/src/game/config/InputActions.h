#pragma once
#include "engine/gba/registers/input/GBAInput.h"

namespace InputActions
{
	using namespace GBA;

	static const Buttons::Enum AdvanceDialogue = Buttons::A;
	static const Buttons::Enum SkipDialogue = Buttons::Start;
	static const Buttons::Enum MoveLeft = Buttons::Left;
	static const Buttons::Enum MoveRight = Buttons::Right;
	static const Buttons::Enum MoveUp = Buttons::Up;
	static const Buttons::Enum MoveDown = Buttons::Down;
	static const Buttons::Enum Interact = Buttons::A;
}
