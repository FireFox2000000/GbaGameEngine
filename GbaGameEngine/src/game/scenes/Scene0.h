#pragma once

#include "engine/scene/Scene.h"

class Scene0 : public Scene
{
public:
	Scene0(Engine* engine);
	~Scene0();

	void Enter(Engine* engine) override;
	void Update(Engine* engine) override;
};
