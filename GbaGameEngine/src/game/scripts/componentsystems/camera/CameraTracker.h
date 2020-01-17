#pragma once
#include "engine/math/geometry/AxisAlignedBoundingBox.h"

class GameObject;

namespace Component
{
	struct CameraTracker
	{
		AxisAlignedBoundingBox2 worldBounds = AxisAlignedBoundingBox2(Vector2<tFixedPoint8>(-9999, -9999), Vector2<tFixedPoint8>(9999, 9999));
		GameObject* objectToTrack = nullptr;
	};
}

class Engine;
namespace System
{
	namespace CameraTracker
	{
		void Update(Engine* engine);
	}
}
