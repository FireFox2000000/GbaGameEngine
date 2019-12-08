#include "MovementTest.h"
#include "engine/engine/engine.h"
#include "engine/gameobject/GameObject.h"
#include "engine/math/Vector2.h"
#include "engine/gba/registers/input/GBAInput.h"
#include "engine/gameobject/transformation/Position.h"
#include "engine/time/Time.h"

void System::PlayerMovement::Update(Engine* engine)
{
	using namespace GBA;
	const Time* time = engine->GetComponent<Time>();
	auto dt = time->GetDtSecondsf();

	auto* entityManager = engine->GetEntityRegistry();
	entityManager->InvokeEach<Component::Position, Component::PlayerMovement>([&dt](Component::Position& position, Component::PlayerMovement& playerMovement)
		{
			float moveSpeed = playerMovement.moveSpeed * dt;

			if (Input::GetKey(Buttons::Left))
			{
				position.x -= moveSpeed;
			}

			if (Input::GetKey(Buttons::Right))
			{
				position.x += moveSpeed;
			}

			if (Input::GetKey(Buttons::Up))
			{
				position.y += moveSpeed;
			}

			if (Input::GetKey(Buttons::Down))
			{
				position.y -= moveSpeed;
			}
		});
}
