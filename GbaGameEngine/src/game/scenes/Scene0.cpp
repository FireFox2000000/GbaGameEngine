#include "Scene0.h"
#include "engine/engine/engine.h"
#include "engine/gba/registers/display/GBADisplayControl.h"

const int totalTestSprites = 90;

Scene0::Scene0(Engine* engine)
	: Scene(engine)
{
}

Scene0::~Scene0()
{
}

void Scene0::Enter(Engine* engine)
{
	using namespace GBA;
	using namespace GBA::DisplayOptions;

	DisplayControl::SetDisplayOptions(Mode0 | Sprites | MappingMode1D);
}

void Scene0::Update(Engine* engine)
{
	Scene::Update(engine);
}
