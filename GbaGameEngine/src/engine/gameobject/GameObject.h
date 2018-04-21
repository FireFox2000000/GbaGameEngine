#ifndef PRAGMA_ONCE_ENGINE_GAMEOBJECT_GAMEOBJECT_H
#define PRAGMA_ONCE_ENGINE_GAMEOBJECT_GAMEOBJECT_H

#include "engine/math/Vector2.h"

// TODO: replace with transformation matrix
// Component system

namespace GBA
{
	class ObjectAttribute;
	typedef volatile ObjectAttribute vObjectAttribute;
}

class GameObject
{
	Vector2 m_localPosition;	

public:
	GameObject();
	~GameObject();

	inline Vector2 GetPosition2() { return m_localPosition; }
	inline void SetPosition(const Vector2& position) { m_localPosition = position; }

	void Update();
};

#endif
