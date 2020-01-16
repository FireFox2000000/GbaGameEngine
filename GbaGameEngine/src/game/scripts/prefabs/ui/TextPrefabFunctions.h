#pragma once
#include "engine/asset/libraries/FontLibrary.h"

class Engine;
class GameObject;

namespace TextPrefabFunctions
{
	void MakeBasicTextObj(Engine* engine, GameObject& out_go, FontID::Enum font);
}
