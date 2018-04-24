#include "SceneManager.h"
#include "engine/base/Macros.h"

SceneManager::SceneManager(Scene* initialScene)
	: m_current(initialScene)
{
}


SceneManager::~SceneManager()
{
}

