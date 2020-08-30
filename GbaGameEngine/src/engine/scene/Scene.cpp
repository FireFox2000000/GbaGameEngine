#include "Scene.h"
#include "engine/engine/engine.h"
#include "engine/gameobject/Camera.h"
#include "engine/render/SpriteRenderer.h"
#include "engine/render/TextRenderer.h"
#include "engine/render/TilemapRenderer.h"
#include "engine/graphicalassets/Graphics.h"

//#define RENDER_PROFILE
#ifdef RENDER_PROFILE
#include "engine/gba/registers/clock/GBATimer.h"
#endif

Scene::Scene(Engine* engine)
	: m_mainCamera()
{
	m_mainCamera.AddComponent<Component::Camera>();
}

Scene::~Scene()
{
}

void Scene::Update(Engine * engine)
{
}

void Scene::PreRender(Engine * engine)
{
#ifdef RENDER_PROFILE
	auto& profilerClock = GBA::Timers::GetTimer(GBA::Timers::Profile);
	profilerClock.SetFrequency(GBA::Timers::Cycle_64);
	profilerClock.SetActive(true);
#endif
	System::SpriteRenderer::Render(engine, &m_mainCamera);
#ifdef RENDER_PROFILE
	DEBUG_LOGFORMAT("[Profile Prerender System::SpriteRenderer::Render] = %d", profilerClock.GetCurrentTimerCount());
	profilerClock.SetActive(false);

	profilerClock.SetActive(true);
#endif
	System::UI::TextRenderer::Render(engine);
#ifdef RENDER_PROFILE
	DEBUG_LOGFORMAT("[Profile Prerender System::UI::TextRenderer::Render] = %d", profilerClock.GetCurrentTimerCount());
	profilerClock.SetActive(false);
#endif
}

void Scene::Render(Engine* engine)
{
	// Todo, this just renders sprite, should also do the vblank render of tilemaps as seen below
	Graphics* gfx = engine->EditComponent<Graphics>();
	gfx->EndFrame();

	System::TilemapRenderer::VBlankRender(engine, &m_mainCamera);
}
