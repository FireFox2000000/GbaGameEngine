#ifndef PRAGMA_ONCE_ENGINE_SCENE_SCENE_H
#define PRAGMA_ONCE_ENGINE_SCENE_SCENE_H

class Scene
{
	friend class SceneManager;

	virtual void Update() {};
	virtual void Render() {};

public:
	Scene();
	virtual ~Scene();
};

#endif
