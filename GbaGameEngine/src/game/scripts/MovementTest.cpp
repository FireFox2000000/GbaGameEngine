#include "MovementTest.h"
#include "engine/engine/engine.h"
#include "engine/gameobject/GameObject.h"
#include "engine/math/Vector2.h"
#include "engine/gba/registers/input/GBAInput.h"
#include "engine/gameobject/transformation/Transform.h"
#include "engine/time/Time.h"

void System::PlayerMovement::Update(Engine* engine)
{
	using namespace GBA;
	const Time* time = engine->GetComponent<Time>();

	auto dt = tFixedPoint24(time->GetDtSecondsf());

	auto* entityManager = engine->GetEntityRegistry();
	entityManager->InvokeEach<Component::Transform, Component::PlayerMovement>([&dt](Component::Transform& transform, Component::PlayerMovement& playerMovement)
		{
			tFixedPoint8 moveSpeed = (tFixedPoint8)(playerMovement.moveSpeed * dt);
			auto position = transform.GetPosition();
			auto scale = transform.GetScale();

			if (Input::GetKey(Buttons::Left))
			{
				position.x -= moveSpeed;
				scale.x = -1;
			}

			if (Input::GetKey(Buttons::Right))
			{
				position.x += moveSpeed;
				scale.x = 1;
			}

			if (Input::GetKey(Buttons::Up))
			{
				position.y += moveSpeed;
			}

			if (Input::GetKey(Buttons::Down))
			{
				position.y -= moveSpeed;
			}

			transform.SetPosition(position);
			transform.SetScale(scale);
		});
}
