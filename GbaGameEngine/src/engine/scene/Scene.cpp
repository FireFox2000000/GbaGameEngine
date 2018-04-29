#include "Scene.h"
#include "engine\rendering\Renderer.h"

Scene::Scene()
{
}


Scene::~Scene()
{
}

void Scene::Update()
{
	for (List<GameObject>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		it->Update();
	}

	mainCamera.Update();
}

void Scene::Render()
{
	for (u32 i = 0; i < renderList.Count(); ++i)
	{
		renderList[i]->Render(&mainCamera);
	}
}
