#pragma once
#include "engine/math/geometry/AxisAlignedBoundingBox.h"
#include <functional>

struct GameRulestateParams;
class GameObject;

namespace Component
{
	struct RpgInteractor
	{
		AxisAlignedBoundingBox2 interactionTrigger;
		Vector2<tFixedPoint8> facingProjectionOffset;
	};

	struct RpgInteractable
	{
		// Callback fn
		std::function<void(GameObject* interactor, GameRulestateParams& params)> onInteracted;
	};
}

namespace Component
{
	struct Transform;
	struct Collider;
	struct RpgMovement;
}

namespace System
{
	namespace RpgInteraction
	{
		void TryInteract(
			GameObject* interactorObject,
			GameRulestateParams& params);
	}
}
