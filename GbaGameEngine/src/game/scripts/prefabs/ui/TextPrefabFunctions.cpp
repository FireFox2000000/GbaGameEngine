#include "TextPrefabFunctions.h"
#include "engine/gameobject/transformation/Transform.h"
#include "engine/gameobject/ui/ScreenTransform.h"
#include "engine/gameobject/ui/Text.h"
#include "engine/gameobject/GameObject.h"
#include "engine/engine/engine.h"

void TextPrefabFunctions::MakeBasicTextObj(Engine* engine, GameObject & out_go, FontID::Enum font)
{
	out_go.RemoveComponent<Component::Transform>();

	FontLibrary* fontLibrary = engine->EditComponent<FontLibrary>();

	auto& screenTransform = out_go.AddComponent<Component::UI::ScreenTransform>();
	screenTransform.position.x = 0;
	screenTransform.position.y = 0;

	auto& textComponent = out_go.AddComponent<Component::UI::Text>();
	textComponent.m_font = fontLibrary->GetFont(font);
	textComponent.m_str = std::string("");
}
