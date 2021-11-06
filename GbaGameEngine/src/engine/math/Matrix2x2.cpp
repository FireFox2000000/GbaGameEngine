#include "Matrix2x2.h"
#include "engine/math/Math.h"

// https://www.coranac.com/tonc/text/affine.htm
// Note that this is the human-correct matrix equation. We  do invertion for the GBA in another step before this
void Matrix2x2::SetTransformation(Vector2<tFixedPoint8> scale, u16 rotationAlpha)
{
	int sinResult = Math::Sin(rotationAlpha), cosSinResult = Math::Cos(rotationAlpha);

	constexpr int shift = SIN_LUT_FRACTIONAL_BITS;

	a.SetStorage(cosSinResult * scale.x.GetStorage() >> shift);
	b.SetStorage(sinResult * scale.x.GetStorage() >> shift);
	c.SetStorage(-sinResult * scale.y.GetStorage() >> shift);
	d.SetStorage(cosSinResult * scale.y.GetStorage() >> shift);
}
