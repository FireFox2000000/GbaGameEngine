#include "Scene0.h"
#include "engine/engine/engine.h"
#include "engine/gba/registers/display/GBADisplayControl.h"
#include "game/scripts/rulestates/GeneralGameplay_Rulestate.h"
#include "engine/base/core/stl/SharedPtr.h"
#include "engine/asset/libraries/TilemapLibrary.h"
#include "engine/render/TilemapRenderer.h"
#include "engine/gameobject/transformation/Transform.h"
#include "game/scripts/prefabs/game/PlayerPrefab.h"
#include "game/scripts/componentsystems/camera/CameraTracker.h"
#include "game/scripts/componentsystems/movement/RpgMovement.h"
#include "game/scripts/componentsystems/PlayerComponent.h"
#include "game/scripts/componentsystems/collision/Collider.h"

Scene0::Scene0(Engine* engine)
	: Scene(engine)
{
}

Scene0::~Scene0()
{
}

void Scene0::Enter(Engine* engine)
{
	DEBUG_LOG("Entered scene [Scene0]");

	using namespace GBA;
	using namespace GBA::DisplayOptions;

	DisplayControl::SetDisplayOptions(Mode0 | Sprites | MappingMode1D);

	TilemapLibrary* tilemapLib = engine->EditComponent<TilemapLibrary>();
	Tilemap* testBg = tilemapLib->GetTilemap(TilemapSetID::BgTomb, 0);
	auto halfBgSize = testBg->GetSizeInTiles() / 2;

	{
		background = std::make_unique<GameObject>(engine);
		Component::TilemapRenderer& tilemapRenderer = background->AddComponent<Component::TilemapRenderer>();
		tilemapRenderer.SetTilemap(testBg);
		tilemapRenderer.AssignBackgroundSlot();
		Component::Collider& backgroundCollider = background->AddComponent<Component::Collider>();
		backgroundCollider.shape = AxisAlignedBoundingBox2(Vector2<tFixedPoint8>(-halfBgSize.x, -halfBgSize.y), Vector2<tFixedPoint8>(halfBgSize.x, halfBgSize.y));
		backgroundCollider.shapeInverted = true;
	}

	player = std::make_unique<GameObject>(engine);
	PlayerPrefab::MakePlayerObj(engine, *player);

	
	Component::CameraTracker& cameraTracker = m_mainCamera.AddComponent<Component::CameraTracker>();
	cameraTracker.objectToTrack = player.get();
	cameraTracker.worldBounds = AxisAlignedBoundingBox2(Vector2<tFixedPoint8>(-halfBgSize.x, -halfBgSize.y), Vector2<tFixedPoint8>(halfBgSize.x, halfBgSize.y));

	testCollider = std::make_unique<GameObject>(engine);
	PlayerPrefab::MakePlayerObj(engine, *testCollider);
	testCollider->EditComponent<Component::Transform>()->SetPosition(0, 5);
	testCollider->RemoveComponent<Component::RpgMovement>();
	testCollider->RemoveComponent<Component::Player>();

	GameRulestateParams updateParams;
	updateParams.engine = engine;
	updateParams.stateMachine = &m_rulestateMachine;

	m_rulestateMachine.ChangeState<GeneralGameplay_Rulestate>(updateParams);
}

void Scene0::Update(Engine* engine)
{
	GameRulestateParams updateParams;
	updateParams.engine = engine;
	updateParams.stateMachine = &m_rulestateMachine;

	m_rulestateMachine.Update(updateParams);

	Scene::Update(engine);

	System::CameraTracker::Update(engine);
}
