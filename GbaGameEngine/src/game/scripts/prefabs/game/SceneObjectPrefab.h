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
	void MakeFairyProp(Engine* engine, GameObject& out_go);

	enum Direction
	{
		Left,
		Right,
		Up,
		Down,
	};
	void SetReimuPropDirection(Engine* engine, GameObject& prop, Direction dir);

	enum RodColour
	{
		Default,
		Gold,
		Yellow
	};

	void MakePurityRodProp(Engine* engine, GameObject& out_go, RodColour colour);

}