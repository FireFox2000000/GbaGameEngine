#include "Scene0.h"
#include "engine/engine/engine.h"
#include "engine/gba/registers/display/GBADisplayControl.h"
#include "game/scripts/rulestates/GeneralGameplay_Rulestate.h"
#include "engine/base/core/stl/SharedPtr.h"
#include "engine/asset/libraries/TilemapLibrary.h"
#include "engine/render/TilemapRenderer.h"
#include "engine/gameobject/transformation/Transform.h"
#include "game/scripts/prefabs/game/PlayerPrefab.h"
#include "game/scripts/prefabs/game/SceneObjectPrefab.h"
#include "game/scripts/componentsystems/camera/CameraTracker.h"
#include "game/scripts/componentsystems/collision/Collider.h"
#include "engine/asset/libraries/SpriteLibrary.h"
#include "game/scripts/componentsystems/interaction/RpgInteraction.h"
#include "game/scripts/componentsystems/kokirirespawn/RespawnMesh.h"

#include "game/scripts/rulestates/GeneralGameplay_Rulestate.h"
#include "game/scripts/rulestates/Dialogue_Rulestate.h"

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

	SetupSceneProps(engine);

	GameRulestateParams updateParams;
	updateParams.engine = engine;
	updateParams.stateMachine = &m_rulestateMachine;
	updateParams.playerObject = player.get();

	m_rulestateMachine.ChangeState<GeneralGameplay_Rulestate>(updateParams);
}

void Scene0::Update(Engine* engine)
{
	GameRulestateParams updateParams;
	updateParams.engine = engine;
	updateParams.stateMachine = &m_rulestateMachine;
	updateParams.playerObject = player.get();

	m_rulestateMachine.Update(updateParams);

	Scene::Update(engine);

	System::CameraTracker::Update(engine);
}

void Scene0::SetupSceneProps(Engine * engine)
{
	TilemapLibrary* tilemapLib = engine->EditComponent<TilemapLibrary>();
	Tilemap* testBg = tilemapLib->GetTilemap(TilemapSetID::BgForest, 0);
	auto halfBgSize = testBg->GetSizeInTiles() / 2;

	{
		background = std::make_unique<GameObject>(engine);
		Component::TilemapRenderer& tilemapRenderer = background->AddComponent<Component::TilemapRenderer>();
		tilemapRenderer.SetTilemap(testBg);
		tilemapRenderer.AssignBackgroundSlot();
		//Component::Collider& backgroundCollider = background->AddComponent<Component::Collider>();
		//backgroundCollider.shape = AxisAlignedBoundingBox2(Vector2<tFixedPoint8>(-halfBgSize.x, -halfBgSize.y), Vector2<tFixedPoint8>(halfBgSize.x, halfBgSize.y));
		//backgroundCollider.shapeInverted = true;
	}

	{
		respawnGrid = std::make_unique<GameObject>(engine);
		tRespawnMesh::tMesh respawnGridMesh = {		// Possibly inverted on the Y???
			tRespawnMesh::tMeshRow { tRespawnMesh::Box, tRespawnMesh::Box, tRespawnMesh::Box, tRespawnMesh::Box, tRespawnMesh::Box, },
			tRespawnMesh::tMeshRow { tRespawnMesh::Box, tRespawnMesh::Box, tRespawnMesh::None, tRespawnMesh::None, tRespawnMesh::Box, },
			tRespawnMesh::tMeshRow { tRespawnMesh::Box, tRespawnMesh::None, tRespawnMesh::None, tRespawnMesh::Box, tRespawnMesh::Box, },
			tRespawnMesh::tMeshRow { tRespawnMesh::Box, tRespawnMesh::None, tRespawnMesh::Box, tRespawnMesh::None, tRespawnMesh::Box, },
			tRespawnMesh::tMeshRow { tRespawnMesh::Box, tRespawnMesh::Box, tRespawnMesh::Box, tRespawnMesh::Box, tRespawnMesh::Box, },
		};
		
		int gridScale = 64;
		int gridRespawnPointX = 3;
		int gridRespawnPointY = 1;

		tRespawnMesh respawnMesh = tRespawnMesh(respawnGridMesh, true, gridScale);
		respawnGrid->AddComponent<RespawnMesh>(respawnMesh, gridRespawnPointX, gridRespawnPointY);
		respawnGrid->EditComponent<Component::Transform>()->SetPosition(-gridRespawnPointX * gridScale - gridScale / 2, -gridRespawnPointY * gridScale - gridScale / 2);
	}

	player = std::make_unique<GameObject>(engine);
	PlayerPrefab::MakePlayerObj(engine, *player);
	player->EditComponent<Component::Transform>()->SetPosition(0, -5);

	Component::CameraTracker& cameraTracker = m_mainCamera.AddComponent<Component::CameraTracker>();
	cameraTracker.objectToTrack = player.get();
	//cameraTracker.worldBounds = AxisAlignedBoundingBox2(Vector2<tFixedPoint8>(-halfBgSize.x, -halfBgSize.y), Vector2<tFixedPoint8>(halfBgSize.x, halfBgSize.y));

	{
		GameObject* prop = propObjects.AddNew(engine);
		SceneObjectPrefab::MakeReimuProp(engine, *prop);

		prop->EditComponent<Component::Transform>()->SetPosition(0, 0);

		auto* interactable = prop->EditComponent<Component::RpgInteractable>();
		interactable->onInteracted = [this, engine](GameObject* interactor, GameRulestateParams& params)
		{
			if (this->m_sceneFlags.TestBit(SceneFlags::Prop1Interacted))
				return;

			std::string script;
			script += "...";
			script += Dialogue_Rulestate::c_dialogueBoxStepFlag;

			script += "The imposter stares back at you";

			SharedPtr<GameRulestate> dialogueRulestate = std::make_shared<Dialogue_Rulestate>(script, 2, std::make_shared<GeneralGameplay_Rulestate>(), 
				[this, engine]() {
					// On finished callback
					SceneObjectPrefab::SetReimuPropDirection(engine, this->propObjects[0], SceneObjectPrefab::Left);
					this->m_sceneFlags.SetBit(SceneFlags::Prop1Interacted);	// Oneshot
				});
			params.stateMachine->ChangeState(dialogueRulestate, params);
		};
	}

	{
		GameObject* prop = propObjects.AddNew(engine);
		SceneObjectPrefab::MakeReimuProp(engine, *prop);

		prop->EditComponent<Component::Transform>()->SetPosition(-64, 0);

		auto* interactable = prop->EditComponent<Component::RpgInteractable>();
		interactable->onInteracted = [this, engine](GameObject* interactor, GameRulestateParams& params)
		{
			if (this->m_sceneFlags.TestBit(SceneFlags::Prop2Interacted))
				return;

			std::string script;
			script += "...";
			script += Dialogue_Rulestate::c_dialogueBoxStepFlag;
			script += "\"Leave...\"";

			SharedPtr<GameRulestate> dialogueRulestate = std::make_shared<Dialogue_Rulestate>(script, 2, std::make_shared<GeneralGameplay_Rulestate>(),
				[this, engine]() {
					// On finished callback
					SceneObjectPrefab::SetReimuPropDirection(engine, this->propObjects[1], SceneObjectPrefab::Up);
					this->m_sceneFlags.SetBit(SceneFlags::Prop2Interacted);	// Oneshot
				});
			params.stateMachine->ChangeState(dialogueRulestate, params);
		};
	}

	{
		GameObject* prop1 = propObjects.AddNew(engine);
		SceneObjectPrefab::MakeReimuProp(engine, *prop1);
		prop1->EditComponent<Component::Transform>()->SetPosition(-62, 64);

		auto* interactable1 = prop1->EditComponent<Component::RpgInteractable>();
		interactable1->onInteracted = [this, engine](GameObject* interactor, GameRulestateParams& params)
		{
			if (!this->m_sceneFlags.TestBit(SceneFlags::Prop3Interacted))
			{
				std::string script;
				script += "...";
				script += Dialogue_Rulestate::c_dialogueBoxStepFlag;
				script += "\"You must choose...\"";

				SharedPtr<GameRulestate> dialogueRulestate = std::make_shared<Dialogue_Rulestate>(script, 2, std::make_shared<GeneralGameplay_Rulestate>(),
					[this, engine]() {
						// On finished callback
						SceneObjectPrefab::SetReimuPropDirection(engine, this->propObjects[2], SceneObjectPrefab::Right);
						this->m_sceneFlags.SetBit(SceneFlags::Prop3Interacted);	// Oneshot

						SceneObjectPrefab::SetReimuPropDirection(engine, this->propObjects[3], SceneObjectPrefab::Left);
						this->m_sceneFlags.SetBit(SceneFlags::Prop3Interacted);	// Oneshot
					});
				params.stateMachine->ChangeState(dialogueRulestate, params);
			}
			else
			{
				std::string script;
				script += "Your mind is filled with images of sake";

				SharedPtr<GameRulestate> dialogueRulestate = std::make_shared<Dialogue_Rulestate>(script, 2, std::make_shared<GeneralGameplay_Rulestate>());
				params.stateMachine->ChangeState(dialogueRulestate, params);
			}
		};

		GameObject* prop2 = propObjects.AddNew(engine);
		SceneObjectPrefab::MakeReimuProp(engine, *prop2);
		prop2->EditComponent<Component::Transform>()->SetPosition(-66, 64);

		auto* interactable2 = prop2->EditComponent<Component::RpgInteractable>();
		interactable2->onInteracted = [this, engine](GameObject* interactor, GameRulestateParams& params)
		{
			if (!this->m_sceneFlags.TestBit(SceneFlags::Prop3Interacted))
			{
				std::string script;
				script += "...";
				script += Dialogue_Rulestate::c_dialogueBoxStepFlag;
				script += "\"You must choose...\"";

				SharedPtr<GameRulestate> dialogueRulestate = std::make_shared<Dialogue_Rulestate>(script, 2, std::make_shared<GeneralGameplay_Rulestate>(),
					[this, engine]() {
						// On finished callback
						SceneObjectPrefab::SetReimuPropDirection(engine, this->propObjects[2], SceneObjectPrefab::Right);
						this->m_sceneFlags.SetBit(SceneFlags::Prop3Interacted);	// Oneshot

						SceneObjectPrefab::SetReimuPropDirection(engine, this->propObjects[3], SceneObjectPrefab::Left);
						this->m_sceneFlags.SetBit(SceneFlags::Prop3Interacted);	// Oneshot
					});
				params.stateMachine->ChangeState(dialogueRulestate, params);
			}
			else
			{
				std::string script;
				script += "Your mind is filled with images of pancakes";

				SharedPtr<GameRulestate> dialogueRulestate = std::make_shared<Dialogue_Rulestate>(script, 2, std::make_shared<GeneralGameplay_Rulestate>());
				params.stateMachine->ChangeState(dialogueRulestate, params);
			}
		};
	}

	{
		GameObject* rodProp1 = propObjects.AddNew(engine);
		SceneObjectPrefab::MakePurityRodProp(engine, *rodProp1, SceneObjectPrefab::RodColour::Default);
		rodProp1->EditComponent<Component::Transform>()->SetPosition(-128, 64);
	}
}
