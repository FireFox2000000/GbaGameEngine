#include "GBAAttributeFunctions.h"
#include "engine/math/Math.h"

Vector2 GetSquareShape(GBA::Attributes::SizeMode sizeMode)
{
	return Vector2(1, 1) * pow(2.f, sizeMode);
}

Vector2 GetWideShape(GBA::Attributes::SizeMode sizeMode)
{
	using namespace GBA::Attributes;

	switch(sizeMode)
	{
	case(Form0):
		return Vector2(2, 1);

	case (Form1):
		return Vector2(4, 1);

	case (Form2):
		return Vector2(4, 2);

	case(Form3):
		return Vector2(8, 4);

	default:
		break;
	}

	return Vector2::Zero;
}

Vector2 GetTallShape(GBA::Attributes::SizeMode sizeMode)
{
	using namespace GBA::Attributes;

	switch (sizeMode)
	{
	case(Form0):
		return Vector2(1, 2);

	case (Form1):
		return Vector2(1, 4);

	case (Form2):
		return Vector2(2, 4);

	case(Form3):
		return Vector2(4, 8);

	default:
		break;
	}

	return Vector2::Zero;
}

Vector2 GBA::AttributeFunctions::GetTileSize(Attributes::Shape shape, Attributes::SizeMode sizeMode)
{
	using namespace Attributes;

	switch (shape)
	{
	case(Square):
		return GetSquareShape(sizeMode);

	case(Wide):
		return GetWideShape(sizeMode);

	case(Tall):
		return GetTallShape(sizeMode);

	default:
		break;
	}

	return Vector2::Zero;
}

Vector2 GBA::AttributeFunctions::GetPixelSize(Attributes::Shape shape, Attributes::SizeMode sizeMode)
{
	const int c_pixelsPerTile = 8;

	return GetTileSize(shape, sizeMode) * c_pixelsPerTile;
}
