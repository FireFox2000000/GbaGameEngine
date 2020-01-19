#include "ConclusionScene.h"
#include "engine/engine/engine.h"
#include "engine/gba/registers/display/GBADisplayControl.h"
#include "game/scripts/rulestates/Dialogue_Rulestate.h"
#include "game/scripts/prefabs/game/PlayerPrefab.h"
#include "game/scripts/prefabs/game/SceneObjectPrefab.h"
#include "engine/gameobject/transformation/Transform.h"
#include "engine/render/SpriteRenderer.h"

ConclusionScene::ConclusionScene(Engine * engine) : Scene(engine)
{
}

ConclusionScene::~ConclusionScene()
{
}

void ConclusionScene::Enter(Engine * engine)
{
	DEBUG_LOG("Entered scene [ConclusionScene]");

	using namespace GBA;
	using namespace GBA::DisplayOptions;

	DisplayControl::SetDisplayOptions(Mode0 | Sprites | MappingMode1D);

	GameRulestateParams updateParams;
	updateParams.engine = engine;
	updateParams.stateMachine = &m_rulestateMachine;

	m_player = std::make_unique<GameObject>(engine);

	PlayerPrefab::MakePlayerObj(engine, *m_player);
	SceneObjectPrefab::SetReimuPropDirection(engine, *m_player, SceneObjectPrefab::Direction::Up);

	m_npcFairy = std::make_unique<GameObject>(engine);
	m_npcFairy->EditComponent<Component::Transform>()->SetPosition(0, 5);

	NextCutsceneCommand(m_dialogueIndex++, updateParams);
}

void ConclusionScene::Update(Engine * engine)
{
	GameRulestateParams updateParams;
	updateParams.engine = engine;
	updateParams.stateMachine = &m_rulestateMachine;

	m_rulestateMachine.Update(updateParams);

	if (!m_rulestateMachine.HasState())
	{
		NextCutsceneCommand(m_dialogueIndex++, updateParams);
	}

	Scene::Update(engine);
}

void ConclusionScene::NextCutsceneCommand(int index, GameRulestateParams& params)
{
	DEBUG_LOGFORMAT("[ConclusionScene] StartDialogue %d", index);
	Engine* engine = params.engine;

	if (index == 0)
	{
		std::string script = "Reimu grasps the rod. Divine power courses through her fingertips before settling down again.";
		script += Dialogue_Rulestate::c_dialogueBoxStepFlag;
		script += "(Voice): \"Well done, shrine maiden. I suppose you are...purer then you look. Use the rod...wisely.\"";
		script += Dialogue_Rulestate::c_dialogueBoxStepFlag;
		script += "The voice dies away into the dark recesses of the woods. Slowly, the branches overhead begin to part. Sunlight floods the forest floor once more, and a path to the sky opens up. Before she leaves, Reimu hears a loud wailing coming her way. She spots a fairy yelling right at her.";
		script += Dialogue_Rulestate::c_dialogueBoxStepFlag;
		
		m_rulestateMachine.ChangeState<Dialogue_Rulestate>(params, script, 4, nullptr);
	}
	else if (index == 1)
	{
		SceneObjectPrefab::SetReimuPropDirection(engine, *this->m_player, SceneObjectPrefab::Direction::Down);
		SceneObjectPrefab::MakeFairyProp(engine, *this->m_npcFairy);

		std::string script;
		script += "(Fairy): \"Wait! Wait! Take me with you! I don't wanna be trapped here forever! Let me out! Let me oooouuuuutttt!\"";
		script += Dialogue_Rulestate::c_dialogueBoxStepFlag;

		m_rulestateMachine.ChangeState<Dialogue_Rulestate>(params, script, 4, nullptr);
	}
	else if (index == 2)
	{
		SceneObjectPrefab::SetReimuPropDirection(engine, *this->m_player, SceneObjectPrefab::Direction::Up);

		std::string script;
		script += "The bumbling fairy falls to her knees begging the shrine maiden.";
		script += Dialogue_Rulestate::c_dialogueBoxStepFlag;
		script += "(Reimu): \"Are you alright? How did you get here!?\"";
		script += Dialogue_Rulestate::c_dialogueBoxStepFlag;
		script += "(Fairy): \"Oh hi, sorry. My name is Auko. I'm fine, thank you. Honestly, I donÅft know how I got here! Or how long I've been here! You see it all started with this ice fairy who told me about this stick-\"";
		script += Dialogue_Rulestate::c_dialogueBoxStepFlag;
		script += "(Reimu): \"I get the idea. Now, if I let you out with me, you won't go around starting trouble right?\"";
		script += Dialogue_Rulestate::c_dialogueBoxStepFlag;
		script += "(Auko): \"Of course! I'll do whatever you want just get me out of here!\"";

		m_rulestateMachine.ChangeState<Dialogue_Rulestate>(params, script, 4, nullptr);
	}
	else if (index == 3)
	{
		m_player->EditComponent<Component::SpriteRenderer>()->SetSprite(nullptr);
		m_npcFairy->EditComponent<Component::SpriteRenderer>()->SetSprite(nullptr);

		std::string script;
		script += "Reimu, rod in hand, and Auko fly out of the opening and into the sky above. The sun has begun to set as an orange hue tints the forests and vistas of Gensokyo.";
		m_rulestateMachine.ChangeState<Dialogue_Rulestate>(params, script, 4, nullptr);
	}
	else
	{
		std::string script;
		script += "Thanks for playing!\n\n\nDeveloped by: FireFox\n\nArt by: Susanoo\n\nStory by: Goury";

		m_rulestateMachine.ChangeState<Dialogue_Rulestate>(params, script, 12, nullptr);
	}
}
