#include "GameObject.h"
#include "engine\base\Macros.h"
#include "engine\gba\input\GBAInput.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
	using namespace GBA;

	Vector2 position = GetPosition2();

	if (Input::GetKey(Buttons::Left))
	{
		position.x -= 1;
	}
	
	if (Input::GetKey(Buttons::Right))
	{
		position.x += 1;
	}

	if (Input::GetKey(Buttons::Up))
	{
		position.y += 1;
	}

	if (Input::GetKey(Buttons::Down))
	{
		position.y -= 1;
	}

	SetPosition(position);
}
