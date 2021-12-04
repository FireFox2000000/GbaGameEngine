#include "MovementTest.h"
#include "engine/engine/engine.h"
#include "engine/gameobject/GameObject.h"
#include "engine/math/Vector2.h"
#include "engine/gba/registers/input/GBAInput.h"
#include "engine/gameobject/transformation/Transform.h"
#include "engine/time/Time.h"

void System::PlayerMovement::Update(Engine* engine, GameObject& playerObject)
{
	using namespace GBA;
	const Time* time = engine->GetComponent<Time>();

	auto dt = tFixedPoint24(time->GetDt().ToFloat());

	auto& transform = *playerObject.EditComponent<Component::Transform>();
	const auto& playerMovement = *playerObject.GetComponent<Component::PlayerMovement>();

	tFixedPoint8 moveSpeed = (tFixedPoint8)(playerMovement.moveSpeed * dt);
	auto position = transform.GetPosition();
	auto scale = transform.GetScale();
	//auto rotation = transform.GetRotationDegrees();
	//rotation += (tFixedPoint8)(dt * -90);
	//transform.SetRotationDegrees(rotation);

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
}
