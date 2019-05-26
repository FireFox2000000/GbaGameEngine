#include "Scene.h"
#include "engine/render/SpriteRenderer.h"
#include "engine/graphicalassets/sprite/SpriteManager.h"
#include "engine/engine/engine.h"
#include "engine/gameobject/Camera.h"

Scene::Scene(Engine* engine)
	: m_mainCamera(engine->GetEntityRegistry())
{
	m_mainCamera.AddComponent<Component::Camera>();
}


Scene::~Scene()
{
}

void Scene::Update(Engine* engine)
{
	// Todo, run proper animation systems
}

void Scene::Render(Engine* engine)
{
	// TODO, run rendering systems
	System::SpriteRenderer::Render(engine, &m_mainCamera);
}
