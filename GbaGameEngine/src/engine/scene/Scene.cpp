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
#include "engine/transform/ScreenTransform.h"

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

	System::UpdateSpriteAnimators();
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
		System::RenderSprites(&m_mainCamera);
	}
	{
#ifdef RENDER_PROFILE
		PROFILE_SCOPED_CLOCK_64(Prerender_System_UI_TextRenderer__Render);
#endif
		auto* entityManager = Engine::GetInstance().GetEntityRegistry();
		Graphics* graphics = Engine::GetInstance().GetComponent<Graphics>();

		entityManager->InvokeEach<ScreenTransform, ::UI::TextRenderer>(
			[&graphics]
		(const ScreenTransform& transform, const ::UI::TextRenderer& textRenderer)
			{
				System::UI::RenderTextComponent(*graphics, transform, textRenderer);
			});
	}
}

void Scene::Render()
{
	Graphics* gfx = Engine::GetInstance().GetComponent<Graphics>();
	gfx->EndFrame();

	System::RenderTilemapsVBlank(&m_mainCamera);

	// All our main renderer should be done now. Apply post-processing effects.
	gfx->LateRender();
}
