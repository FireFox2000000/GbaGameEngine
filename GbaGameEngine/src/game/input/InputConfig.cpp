#include "InputConfig.h"
#include "engine/base/core/stl/Array.h"
#include "engine/base/core/stl/List.h"
#include "engine/base/Macros.h"
#include "engine/input/GbaKeypadMap.h"
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
	{ MoveLeft, List<Input::GbaKeypadMap> { Input::GbaKeypadMap(Input::GbaKeypadDevice::Buttons::Left) }},
	{ MoveRight, List<Input::GbaKeypadMap> { Input::GbaKeypadMap(Input::GbaKeypadDevice::Buttons::Right) }},
	{ MoveUp, List<Input::GbaKeypadMap> { Input::GbaKeypadMap(Input::GbaKeypadDevice::Buttons::Up) }},
	{ MoveDown, List<Input::GbaKeypadMap> { Input::GbaKeypadMap(Input::GbaKeypadDevice::Buttons::Down) }},
	{ Jump, List<Input::GbaKeypadMap> { Input::GbaKeypadMap(Input::GbaKeypadDevice::Buttons::A) }},
	{ ExitTilemapTestScene, List<Input::GbaKeypadMap> { Input::GbaKeypadMap(Input::GbaKeypadDevice::Buttons::A) }},
	{ ReturnToLevelSelector, List<Input::GbaKeypadMap> { Input::GbaKeypadMap(Input::GbaKeypadDevice::Buttons::Start) }},
	{ MenuDown, List<Input::GbaKeypadMap> { Input::GbaKeypadMap(Input::GbaKeypadDevice::Buttons::Down) }},
	{ MenuUp, List<Input::GbaKeypadMap> { Input::GbaKeypadMap(Input::GbaKeypadDevice::Buttons::Up) }},
	{ MenuSelect, List<Input::GbaKeypadMap> { Input::GbaKeypadMap(Input::GbaKeypadDevice::Buttons::A) }},
	{ MenuCancel, List<Input::GbaKeypadMap> { Input::GbaKeypadMap(Input::GbaKeypadDevice::Buttons::B) }},
	{ ToggleMusic, List<Input::GbaKeypadMap> { Input::GbaKeypadMap(Input::GbaKeypadDevice::Buttons::R) }},
	{ AdvanceDialogue, List<Input::GbaKeypadMap> { Input::GbaKeypadMap(Input::GbaKeypadDevice::Buttons::A) }},
	{ SkipDialogue, List<Input::GbaKeypadMap> { Input::GbaKeypadMap(Input::GbaKeypadDevice::Buttons::Start) }},
	{ SoftReset, List<Input::GbaKeypadMap> { Input::GbaKeypadMap(Input::GbaKeypadDevice::Buttons::Start | Input::GbaKeypadDevice::Buttons::L | Input::GbaKeypadDevice::Buttons::R) }},
};

Input::InputMapIter InputConfig::GetGameInputIter(GameInputs input, int mapForDeviceId)
{
	// TODO for PC, get inputList via mapForDeviceId

	const auto& inputs = GbaKeypadInputList.inputList[input];

	return Input::InputMapIter{ inputs.begin(), inputs.end(), sizeof(Input::GbaKeypadMap) };
}
