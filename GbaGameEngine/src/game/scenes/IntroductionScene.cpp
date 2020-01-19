#include "IntroductionScene.h"
#include "engine/engine/engine.h"
#include "engine/gba/registers/display/GBADisplayControl.h"
#include "game/scripts/rulestates/Dialogue_Rulestate.h"
#include "engine/scene/SceneManager.h"
#include "game/scenes/Scene0.h"
#include "game/scripts/prefabs/game/PlayerPrefab.h"

IntroductionScene::IntroductionScene(Engine * engine) : Scene(engine)
{
}

IntroductionScene::~IntroductionScene()
{
}

void IntroductionScene::Enter(Engine * engine)
{
	DEBUG_LOG("Entered scene [IntroductionScene]");

	using namespace GBA;
	using namespace GBA::DisplayOptions;

	DisplayControl::SetDisplayOptions(Mode0 | Sprites | MappingMode1D);

	GameRulestateParams updateParams;
	updateParams.engine = engine;
	updateParams.stateMachine = &m_rulestateMachine;

	m_player = std::make_unique<GameObject>(engine);

	PlayerPrefab::MakePlayerObj(engine, *m_player);

	std::string script;
	script += "Reimu Hakurai is but another shrine maiden to pass through these parts. ";
	script += "She is no stranger to the seemingly endless trees that litter the Forest of Magic. ";
	script += "As sunlight dances on the ground and branches sway in the wind, the shrine maiden ventures deeper into the woods. ";
	script += "From what she remembers, much of the forest is shrouded in mystery and superstition.Marisa and Alice live closer to the edge of the forest, but the deeper regions harbor a rather...malign aura. ";
	script += "Nobody in their right mind, not Youkai nor human, would dare enter these areas. The few that found their way there became lost amongst the undergrowth, never to escape.";
	script += Dialogue_Rulestate::c_dialogueBoxStepFlag;
	script += "Reimu presses further into the uncharted reaches of the forest. ";
	script += "The canopy becomes increasingly dense, with the radiant sunlight soon obscured by a mess of leaves. ";
	script += "Beneath one of the trees, she takes a moment to rest. The aura of the forest is beginning to take its effects, but she knows that there is no turning back. ";
	script += "Those who ventured here before knew what they would find: a mysterious Purification Rod. ";
	script += "The rod was rumored to have a legendary power of unknown magnitude, yet those who came in search of it were never to be seen again.";
	script += Dialogue_Rulestate::c_dialogueBoxStepFlag;
	script += "As Reimu stops beneath a tree, an eerie silence befalls the woods. Verdant leaves flutter to the forest floor as the howling of the wind slowly subsides. It is as if the presence of the shrine maiden has gotten the attention of the forest. The trees and bushes fix a hostile gaze at Reimu, awaiting her next move. In the distance, the echoes of the lost can now be heard. These whispers are soon accompanied by the impending voice which roars through the forest.";
	script += Dialogue_Rulestate::c_dialogueBoxStepFlag;
	script += "(Voice): \"Reimu Hakurei...keeper of the Hakurei Shrine...welcome to the Arboretum of Purity. You are not the first to...arrive here...and perhaps you will not be...the last. My identity is...none of your...c-concern\"";
	script += Dialogue_Rulestate::c_dialogueBoxStepFlag;
	script += "(Reimu): \"I don't care what you are. I've come for the purification rod. There's no use-\"";
	script += Dialogue_Rulestate::c_dialogueBoxStepFlag;
	script += "(Voice): \"Silence! You believe you...deserve an item of such power? Such...entitlement. If you truly believe you are...worthy of such an item...well...\"";
	script += Dialogue_Rulestate::c_dialogueBoxStepFlag;
	script += "\"Prove. It. I will present you with a series of...tr-trials to see if you truly are...prepared... for it.\"";
	script += Dialogue_Rulestate::c_dialogueBoxStepFlag;
	script += "\"Succeed, and the rod is...yours. Fail, and like those before you, you shall be...punished to wander the Arboretum for...well...forever. I'm sure there are a few wanderers that are willing to...h-help you.\"";
	script += Dialogue_Rulestate::c_dialogueBoxStepFlag;
	script += "\"I would say 'good luck' but...luck has no use in the Trials of Purity.\"";

	m_rulestateMachine.ChangeState<Dialogue_Rulestate>(updateParams, script, 4, nullptr);
}

void IntroductionScene::Update(Engine * engine)
{
	GameRulestateParams updateParams;
	updateParams.engine = engine;
	updateParams.stateMachine = &m_rulestateMachine;

	m_rulestateMachine.Update(updateParams);

	Scene::Update(engine);

	if (!m_rulestateMachine.HasState())
	{
		engine->EditComponent<SceneManager>()->ChangeScene<Scene0>(engine);
	}
}
