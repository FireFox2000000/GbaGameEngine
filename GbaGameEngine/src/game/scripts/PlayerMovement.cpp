#include "PlayerMovement.h"
#include "engine/engine/engine.h"
#include "engine/gameobject/GameObject.h"
#include "engine/gba/registers/input/GBAInput.h"
#include "engine/gameobject/transformation/Transform.h"
#include "engine/time/Time.h"
#include "game/input/Input.h"

Vector2<int> GetDesiredDirectionFromDpad(Engine* engine)
{
	Vector2<int> desiredDirection;

	Input::InputManager* inputManager = engine->GetComponent<Input::InputManager>();
	const auto& devices = inputManager->GetDevices();

	if (Input::GetInput(MoveLeft, devices))
	{
		desiredDirection = Vector2<int>::Left;
	}
	
	if (Input::GetInput(MoveRight, devices))
	{
		desiredDirection = Vector2<int>::Right;
	}
	
	if (Input::GetInput(MoveUp, devices))
	{
		desiredDirection = Vector2<int>::Up;
	}
	
	if (Input::GetInput(MoveDown, devices))
	{
		desiredDirection = Vector2<int>::Down;
	}

	return desiredDirection;
}

void PlayerMovement::MoveHumanPlayerObject(Engine* engine, GameObject& playerObject)
{
	MovePlayerObject(engine, playerObject, GetDesiredDirectionFromDpad(engine));
}

void PlayerMovement::MovePlayerObject(Engine* engine, GameObject& playerObject, const Vector2<int>& desiredDirection)
{
	if (desiredDirection == Vector2<int>::Zero)
		return;

	const Time* time = engine->GetComponent<Time>();
	tFixedPoint24 dt = time->GetDt();

	auto& transform = *playerObject.EditComponent<Component::Transform>();
	const auto& playerMovement = *playerObject.GetComponent<Component::PlayerMovement>();

	tFixedPoint8 moveSpeed = (tFixedPoint8)(playerMovement.moveSpeed * dt);
	auto position = transform.GetPosition();
	//auto rotation = transform.GetRotationDegrees();
	//rotation += (tFixedPoint8)(dt * -90);
	//transform.SetRotationDegrees(rotation);

	position.x += moveSpeed * desiredDirection.x;
	position.y += moveSpeed * desiredDirection.y;

	transform.SetPosition(position);

	if (desiredDirection.x != 0)
	{
		auto scale = transform.GetScale();
		scale.x = desiredDirection.x;
		transform.SetScale(scale);
	}
}
