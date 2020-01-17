#include "CameraTracker.h"
#include "engine/engine/engine.h"
#include "engine/gameobject/transformation/Transform.h"
#include "engine/gameobject/GameObject.h"
#include "engine/gameobject/Camera.h"

void System::CameraTracker::Update(Engine * engine)
{
	auto* entityManager = engine->GetEntityRegistry();

	entityManager->InvokeEach<Component::Transform, Component::Camera, Component::CameraTracker>(
		[]
	(Component::Transform& transform, Component::Camera& camera, Component::CameraTracker& tracker)
		{
			if (!tracker.objectToTrack)
				return;

			const Component::Transform* trackedObjectTransform = tracker.objectToTrack->GetComponent<Component::Transform>();
			auto position = trackedObjectTransform->GetPosition();

			if (tracker.worldBounds.min != tracker.worldBounds.max)
			{
				auto boundary = tracker.worldBounds;
				AxisAlignedBoundingBox2 orthographicCameraBounds = camera.GetOrthoBounds();
				boundary.max -= orthographicCameraBounds.max;
				boundary.min += orthographicCameraBounds.min;
				position = boundary.Clamp(position);
			}

			transform.SetPosition(position);
		});
}
