#include "engine\math\Vector2.h"
#include "engine\gba\input\GBAInput.h"
#include "engine\gba\sprites\GBAObjectAttribute.h"
#include "engine\gba\graphics\GBAGraphics.h"
#include "engine\base\colour\Colour.h"

int main()
{
	using namespace GBA;

	// Initialisation
	Graphics::SetDisplayParameters(IO_Mode0_Tile | IO_OAM);
	Input::Update();

	while (1)
	{
		// Update input
		Input::Update();

		// Main update

		// Compilation testing, remember to remove

		ObjectAttribute objAttr = ObjectAttribute();
		objAttr.SetShape(Attributes::Shape::Wide);

		Colour col;
		u16 rgb15 = col.RGB15();
		UNUSED(rgb15);
	}

	return 0;
}