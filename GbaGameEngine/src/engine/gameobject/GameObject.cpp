#include "GameObject.h"
#include "engine\base\Macros.h"
#include "engine\gba\registers\input\GBAInput.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
	using namespace GBA;
	float moveSpeed = 0.3f;

	Vector2 position = GetPosition2();

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

	SetPosition(position);
}
