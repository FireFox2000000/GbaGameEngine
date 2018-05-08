#include "Scene.h"
#include "engine\render\Renderer.h"
#include "engine\assets\sprite\SpriteLoader.h"

Scene::Scene()
{
}


Scene::~Scene()
{
	using namespace GBA;
	SpriteLoader* spriteLoader = SpriteLoader::GetCurrent();
	spriteLoader->Clear();
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
