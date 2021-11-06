#include "GBAObjectAffine.h"
#include "engine/math/Math.h"

GBA::Gfx::ObjectAffine::~ObjectAffine()
{
}

// https://www.coranac.com/tonc/text/affine.htm
void GBA::Gfx::AffineTransformationMatrix::SetTransformationMatrix(Vector2<tFixedPoint8> scale, u16 rotationAlpha)
{
	// Maths below treats scale from cpu's POV, so x and y scale down. Invert to correct for this.
	scale.x = tFixedPoint8(1) / scale.x;
	scale.y = tFixedPoint8(1) / scale.y;

	int ss = Math::Sin(rotationAlpha), cc = Math::Cos(rotationAlpha);

	constexpr int shift = SIN_LUT_FRACTIONAL_BITS;

	m_pa.SetStorage(cc * scale.x.GetStorage() >> shift);
	m_pb.SetStorage(-ss * scale.x.GetStorage() >> shift);
	m_pc.SetStorage(ss * scale.y.GetStorage() >> shift);
	m_pd.SetStorage(cc * scale.y.GetStorage() >> shift);
}
