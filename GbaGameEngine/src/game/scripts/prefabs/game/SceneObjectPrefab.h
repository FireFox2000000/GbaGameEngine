#pragma once
#include "engine/math/Vector2.h"
#include "engine/base/core/stl/FixedPoint.h"

class Engine;
class GameObject;
class Sprite;

namespace SceneObjectPrefab
{
	void MakeInteractableObj(Engine* engine, GameObject& out_go, Sprite* sprite, const Vector2<tFixedPoint8>& size);
	void MakeReimuProp(Engine* engine, GameObject& out_go);
}