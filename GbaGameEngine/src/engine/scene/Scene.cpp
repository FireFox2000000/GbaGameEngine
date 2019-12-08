#include "Scene.h"
#include "engine/graphicalassets/sprite/SpriteManager.h"
#include "engine/engine/engine.h"
#include "engine/gameobject/Camera.h"
#include "engine/render/SpriteRenderer.h"

Scene::Scene(Engine* engine)
	: m_mainCamera(engine->GetEntityRegistry())
{
	m_mainCamera.AddComponent<Component::Camera>();
}

Scene::~Scene()
{
}

void Scene::Update(Engine * engine)
{
}

void Scene::Render(Engine* engine)
{
	System::SpriteRenderer::Render(engine, &m_mainCamera);
}
