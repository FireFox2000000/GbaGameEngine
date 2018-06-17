#include "GameObject.h"
#include "engine/base/Macros.h"
#include "engine/gba/registers/input/GBAInput.h"
#include "engine/component/GameObjectComponent.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
	for (u32 i = 0; i < m_components.Count(); ++i)
	{
		delete m_components[i];
	}
}

void GameObject::Update(Engine* engine)
{
	for (u32 i = 0; i < m_monoBehaviourComponents.Count(); ++i)
	{
		m_monoBehaviourComponents[i]->Update(engine);
	}
}
