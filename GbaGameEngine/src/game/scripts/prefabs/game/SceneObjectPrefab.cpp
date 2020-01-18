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

	Sprite* sprite = spriteLibrary->GetSprite(SpriteAtlusID::Reimu, 0);
	SceneObjectPrefab::MakeInteractableObj(engine, out_go, sprite, halfSize);
}
