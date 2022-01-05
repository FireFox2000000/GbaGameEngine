#include "InputConfig.h"
#include "engine/base/core/stl/Array.h"
#include "engine/base/core/stl/List.h"
#include "engine/base/Macros.h"
#include "engine/input/GbaKeypadMap.h"
#include "engine/gba/registers/input/GBAInput.h"
#include "game/input/Input.h"
#include <utility>

using tGameInputList = Array<List<Input::GbaKeypadMap>, GameInputs::Count>;

class SortedInputList
{
public:
	tGameInputList inputList;

	SortedInputList(const std::initializer_list<std::pair<GameInputs, List<Input::GbaKeypadMap>>>& l)
	{
		for (auto& val : l)
		{
			inputList[val.first] = val.second;
		}
	}
};

// GBA input map
const SortedInputList GbaKeypadInputList = {
	{ MoveLeft, List<Input::GbaKeypadMap> { Input::GbaKeypadMap(GBA::Buttons::Enum::Left) }},
	{ MoveRight, List<Input::GbaKeypadMap> { Input::GbaKeypadMap(GBA::Buttons::Enum::Right) }},
	{ MoveUp, List<Input::GbaKeypadMap> { Input::GbaKeypadMap(GBA::Buttons::Enum::Up) }},
	{ MoveDown, List<Input::GbaKeypadMap> { Input::GbaKeypadMap(GBA::Buttons::Enum::Down) }},
	{ ExitTilemapTestScene, List<Input::GbaKeypadMap> { Input::GbaKeypadMap(GBA::Buttons::Enum::A) }},
	{ MenuDown, List<Input::GbaKeypadMap> { Input::GbaKeypadMap(GBA::Buttons::Enum::Down) }},
	{ MenuUp, List<Input::GbaKeypadMap> { Input::GbaKeypadMap(GBA::Buttons::Enum::Up) }},
	{ MenuSelect, List<Input::GbaKeypadMap> { Input::GbaKeypadMap(GBA::Buttons::Enum::A) }},
	{ MenuCancel, List<Input::GbaKeypadMap> { Input::GbaKeypadMap(GBA::Buttons::Enum::B) }},
	{ ToggleMusic, List<Input::GbaKeypadMap> { Input::GbaKeypadMap(GBA::Buttons::Enum::R) }},
	{ SoftReset, List<Input::GbaKeypadMap> { Input::GbaKeypadMap(GBA::Buttons::Enum::Start | GBA::Buttons::Enum::L | GBA::Buttons::Enum::R) }},
};

Input::InputMapIter InputConfig::GetGameInputIter(GameInputs input, int mapForDeviceId)
{
	// TODO for PC, get inputList via mapForDeviceId

	const auto& inputs = GbaKeypadInputList.inputList[input];

	return Input::InputMapIter{ inputs.begin(), inputs.end(), sizeof(Input::GbaKeypadMap) };
}
