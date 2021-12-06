#include "GBAObjectAffine.h"
#include "engine/math/Math.h"
#include "engine/math/Matrix2x2.h"

GBA::Gfx::ObjectAffine::~ObjectAffine()
{
}

void GBA::Gfx::ObjectAffine::SetTransformation(const Matrix2x2& that) volatile
{
	m_pa.SetStorage(that.a.GetStorage());
	m_pb.SetStorage(that.b.GetStorage());
	m_pc.SetStorage(that.c.GetStorage());
	m_pd.SetStorage(that.d.GetStorage());
}
