#pragma once

#include "engine/base/Typedefs.h"
#include "engine/base/Macros.h"
#include "engine/base/core/stl/FixedPoint.h"
#include "engine/math/Vector2.h"
#include "engine/math/Matrix2x2.h"

namespace GBA
{
	namespace Gfx
	{
		using FP8 = FixedPoint<s16, 8>;

		// Rotatable/scable/skewable GBA hardware sprite.
		// The GBA can draw a max of 32 of these at a time.
		// See https://www.coranac.com/tonc/text/affobj.htm for more detail
		class ObjectAffine
		{
		private:
			// fills map to ObjectAttribute properties. DO NOT TOUCH THESE. Only need to fill out pa-pd.
			u16 fill0[3];
			FP8 m_pa;
			u16 fill1[3];
			FP8 m_pb;
			u16 fill2[3];
			FP8 m_pc;
			u16 fill3[3];
			FP8 m_pd;

		public:
			ObjectAffine() : m_pa(0), m_pb(0), m_pc(0), m_pd(0) {}
			~ObjectAffine();

			// Don't trash fills
			inline void SetTransformation(const Matrix2x2& that) volatile {
				m_pa.SetStorage(that.a.GetStorage()); 
				m_pb.SetStorage(that.b.GetStorage()); 
				m_pc.SetStorage(that.c.GetStorage()); 
				m_pd.SetStorage(that.d.GetStorage());
			}
		} ALIGN(4);
	}
}
