#include "Scene.h"
#include "engine/render/Renderer.h"
#include "engine/graphicalassets/sprite/SpriteManager.h"

Scene::Scene(Engine* engine)
{
}


Scene::~Scene()
{
}

void Scene::Update(Engine* engine)
{
	for (List<GameObject>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		it->Update(engine);
	}

	mainCamera.Update(engine);
}

void Scene::Render(Engine* engine)
{
	for (u32 i = 0; i < renderList.Count(); ++i)
	{
		renderList[i]->Render(engine, &mainCamera);
	}
}
