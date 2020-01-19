#include "SceneObjectPrefab.h"
#include "engine/engine/engine.h"

#include "game/scripts/componentsystems/collision/Collider.h"
#include "engine/render/SpriteRenderer.h"
#include "game/scripts/componentsystems/interaction/RpgInteraction.h"
#include "engine/gameobject/GameObject.h"
#include "engine/asset/libraries/SpriteLibrary.h"

void SceneObjectPrefab::MakeInteractableObj(Engine* engine, GameObject& out_go, Sprite* sprite, const Vector2<tFixedPoint8>& size)
{
	Component::SpriteRenderer& testBackgroundRenderer = out_go.AddComponent<Component::SpriteRenderer>();
	testBackgroundRenderer.SetSprite(sprite);

	Component::Collider& collider = out_go.AddComponent<Component::Collider>();
	collider.shape = AxisAlignedBoundingBox2(size * -1, size);

	out_go.AddComponent<Component::RpgInteractable>();
}

void SceneObjectPrefab::MakeReimuProp(Engine * engine, GameObject & out_go)
{
	SpriteLibrary* spriteLibrary = engine->EditComponent<SpriteLibrary>();

	Vector2<tFixedPoint8> halfSize = Vector2<tFixedPoint8>(2, 2) / 2;

	Vector2<tFixedPoint8> collisionOffset = Vector2<tFixedPoint8>(0, -1.f);

	Sprite* sprite = spriteLibrary->GetSprite(SpriteAtlusID::Reimu, 0);
	SceneObjectPrefab::MakeInteractableObj(engine, out_go, sprite, halfSize);

	Component::Collider& collider = *out_go.EditComponent<Component::Collider>();
	Vector2<tFixedPoint8> min = Vector2<tFixedPoint8>(-1 * halfSize + collisionOffset);
	Vector2<tFixedPoint8> max = Vector2<tFixedPoint8>(halfSize + collisionOffset);
	collider.shape = AxisAlignedBoundingBox2(min, max);

	out_go.AddComponent<Component::RpgInteractable>();
}

void SceneObjectPrefab::SetReimuPropDirection(Engine * engine, GameObject & prop, Direction dir)
{
	SpriteLibrary* spriteLibrary = engine->EditComponent<SpriteLibrary>();

	int spriteIndex = 0;
	switch (dir)
	{

	case Direction::Left:
		spriteIndex = 4;
		break;

	case Direction::Right:
		prop.EditComponent<Component::Transform>()->SetScale(Vector2<tFixedPoint8>(-1, 1));
		spriteIndex = 4;
		break;

	case Direction::Up:
		spriteIndex = 8;
		break;

	case Direction::Down:
	default:
		break;
	}
	prop.EditComponent<Component::SpriteRenderer>()->SetSprite(spriteLibrary->GetSprite(SpriteAtlusID::Reimu, spriteIndex));
}

void SceneObjectPrefab::MakePurityRodProp(Engine * engine, GameObject & out_go, RodColour colour)
{
	SpriteLibrary* spriteLibrary = engine->EditComponent<SpriteLibrary>();

	Sprite* sprite = spriteLibrary->GetSprite(SpriteAtlusID::PurityRods, colour);
	Vector2<tFixedPoint8> halfSize = sprite->GetSize() / 2;

	SceneObjectPrefab::MakeInteractableObj(engine, out_go, sprite, halfSize);
}
