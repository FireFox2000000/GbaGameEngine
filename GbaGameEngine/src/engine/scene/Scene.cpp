#include "Scene.h"
#include "engine/engine/engine.h"
#include "engine/gameobject/Camera.h"
#include "engine/animation/SpriteAnimator.h"
#include "engine/graphics/sprite/SpriteRenderer.h"
#include "engine/graphics/ui/TextRenderer.h"
#include "engine/graphics/tilemap/TilemapRenderer.h"
#include "engine/graphics/Graphics.h"
#include "engine/physics/PhysicsResolve.h"
#include "engine/debug/Profiler.h"

//#define RENDER_PROFILE

Scene::Scene()
	: m_mainCamera()
{
	m_mainCamera.AddComponent<Camera>();
}

Scene::~Scene()
{
}

void Scene::LateUpdate()
{
	Graphics* gfx = Engine::GetInstance().GetComponent<Graphics>();

	System::SpriteAnimator::Update();
	gfx->Update();
}

void Scene::FixedUpdate()
{
	System::Physics::UpdateTransforms();
	System::Physics::ResolveCollisions();
}

void Scene::PreRender()
{
	{
#ifdef RENDER_PROFILE
		PROFILE_SCOPED_CLOCK_64(Prerender_System_SpriteRenderer_Render);
#endif
		System::SpriteRenderer::Render(&m_mainCamera);
	}
	{
#ifdef RENDER_PROFILE
		PROFILE_SCOPED_CLOCK_64(Prerender_System_UI_TextRenderer__Render);
#endif
		System::UI::TextRenderer::Render();
	}
}

void Scene::Render()
{
	Graphics* gfx = Engine::GetInstance().GetComponent<Graphics>();
	gfx->EndFrame();

	System::TilemapRenderer::VBlankRender(&m_mainCamera);

	// All our main renderer should be done now. Apply post-processing effects.
	gfx->LateRender();
}
