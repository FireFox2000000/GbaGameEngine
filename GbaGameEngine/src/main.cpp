#include "engine\math\Vector2.h"
#include "engine\gba\input\GBAInput.h"
#include "engine\gba\sprites\GBAObjectAttribute.h"

int main()
{
	// Initialisation
	GBA::Input::Update();

	while (1)
	{
		// Update input
		GBA::Input::Update();

		// Main update

		// Compilation testing

		GBA::ObjectAttribute objAttr = GBA::ObjectAttribute();
		objAttr.SetShape(GBA::Attributes::Shape::Wide);
	}

	return 0;
}