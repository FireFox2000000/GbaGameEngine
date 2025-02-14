#include "PlayerMovement.h"
#include "engine/engine/engine.h"
#include "engine/gameobject/GameObject.h"
#include "engine/transform/Transform2.h"
#include "engine/time/Time.h"
#include "game/input/Input.h"
#include "engine/physics/Rigidbody.h"

Vector2i GetDesiredDirectionFromDpad()
{
	Vector2i desiredDirection;

	Input::InputManager* inputManager = Engine::GetInstance().GetComponent<Input::InputManager>();
	const auto& devices = inputManager->GetDevices();

	if (Input::GetInput(MoveLeft, devices))
	{
		desiredDirection = Vector2i::Left;
	}
	
	if (Input::GetInput(MoveRight, devices))
	{
		desiredDirection = Vector2i::Right;
	}
	
	if (Input::GetInput(MoveUp, devices))
	{
		desiredDirection = Vector2i::Up;
	}
	
	if (Input::GetInput(MoveDown, devices))
	{
		desiredDirection = Vector2i::Down;
	}

	return desiredDirection;
}

void PlayerMovement::MoveHumanPlayerObject(GameObject& playerObject)
{
	MovePlayerObject(playerObject, GetDesiredDirectionFromDpad());
}

void PlayerMovement::MovePlayerObject(GameObject& playerObject, const Vector2i& desiredDirection)
{
	const auto& playerMovement = *playerObject.GetComponent<Component::PlayerMovement>();

	auto& transform = *playerObject.EditComponent<Transform2>();

#if false		// Transform-based movement
	const Time* time = engine->GetComponent<Time>();
	FPi24 dt = time->GetDt();
	
	FPi8 moveSpeed = (FPi8)(playerMovement.moveSpeed * dt);
	auto position = transform.GetPosition();
	
	position.x += moveSpeed * desiredDirection.x;
	position.y += moveSpeed * desiredDirection.y;
	
	transform.SetPosition(position);

#else			// Physics-based movement
	auto& rigidbody = *playerObject.EditComponent<Rigidbody>();

	Input::InputManager* inputManager = Engine::GetInstance().GetComponent<Input::InputManager>();
	const auto& devices = inputManager->GetDevices();
	if (Input::GetInputDown(Jump, devices))
	{
		rigidbody.velocity.y = playerMovement.jumpInitVel;
	}
	else if (Input::GetInputUp(Jump, devices) && rigidbody.velocity.y > 0)
	{
		rigidbody.velocity.y = 0;
	}
	rigidbody.velocity = { playerMovement.moveSpeed * desiredDirection.x, rigidbody.velocity.y };
#endif
	{


		//auto rotation = transform.GetRotationDegrees();
		//rotation += (FPi8)(dt * -90);
		//transform.SetRotationDegrees(rotation);

		if (desiredDirection.x != 0)
		{
			auto scale = transform.GetScale();
			scale.x = desiredDirection.x;
			transform.SetScale(scale);
		}
	}
}
