#include "MovementTest.h"
#include "engine\gameobject\GameObject.h"
#include "engine\math\Vector2.h"
#include "engine\gba\registers\input\GBAInput.h"

MovementTest::MovementTest(GameObject * gameObject)
	: MonoBehaviour(gameObject)
{
}

void MovementTest::Update(Engine* engine)
{
	using namespace GBA;
	float moveSpeed = 0.2f;

	Vector2 position = GetGameObject()->GetPosition2();

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

	GetGameObject()->SetPosition(position);
}
