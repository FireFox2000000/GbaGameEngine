#include "engine\math\Vector2.h"
#include "engine\gba\input\GBAInput.h"
#include "engine\gba\sprites\GBAObjectAttribute.h"
#include "engine\gba\displayregisters\GBADisplayControl.h"
#include "engine\base\colour\Colour.h"

int main()
{
	using namespace GBA;

	{
		// Initialisation

		using namespace DisplayOptions;	
		DisplayControl::SetDisplayOptions(Mode0 | Sprites);
	}

	Input::Update();

	while (1)
	{
		// Update input
		Input::Update();

		// Main update

		// Compilation testing, remember to remove

		ObjectAttribute objAttr = ObjectAttribute();
		objAttr.SetShape(Attributes::Shape::Wide);

		Colour16 col = Colour16(31, 31, 31);
		u16 rgb16 = col.RGB16();
		UNUSED(rgb16);
	}

	return 0;
}