#include "Scene.h"
#include "engine/engine/engine.h"
#include "engine/gameobject/Camera.h"
#include "engine/animation/SpriteAnimator.h"
#include "engine/render/SpriteRenderer.h"
#include "engine/render/TextRenderer.h"
#include "engine/render/TilemapRenderer.h"
#include "engine/graphics/Graphics.h"
#include "engine/physics/PhysicsResolve.h"

//#define RENDER_PROFILE
#ifdef RENDER_PROFILE
#include "engine/gba/registers/clock/GBATimer.h"
#endif

Scene::Scene()
	: m_mainCamera()
{
	m_mainCamera.AddComponent<Component::Camera>();
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
#ifdef RENDER_PROFILE
	auto& profilerClock = GBA::Timers::GetTimer(GBA::Timers::Profile);
	profilerClock.SetFrequency(GBA::Timers::Cycle_64);
	profilerClock.SetActive(true);
#endif
	System::SpriteRenderer::Render(&m_mainCamera);
#ifdef RENDER_PROFILE
	DEBUG_LOGFORMAT("[Profile Prerender System::SpriteRenderer::Render] = %d", profilerClock.GetCurrentTimerCount());
	profilerClock.SetActive(false);

	profilerClock.SetActive(true);
#endif
	System::UI::TextRenderer::Render();
#ifdef RENDER_PROFILE
	DEBUG_LOGFORMAT("[Profile Prerender System::UI::TextRenderer::Render] = %d", profilerClock.GetCurrentTimerCount());
	profilerClock.SetActive(false);
#endif
}

void Scene::Render()
{
	Graphics* gfx = Engine::GetInstance().GetComponent<Graphics>();
	gfx->EndFrame();

	System::TilemapRenderer::VBlankRender(&m_mainCamera);

	// All our main renderer should be done now. Apply post-processing effects.
	gfx->LateRender();
}
