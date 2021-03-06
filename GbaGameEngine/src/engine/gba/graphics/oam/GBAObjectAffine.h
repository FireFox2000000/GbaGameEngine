#pragma once

#include "engine/base/Typedefs.h"
#include "engine/base/Macros.h"

namespace GBA
{
	namespace Gfx
	{
		// TODO, not implemented yet.
		// Rotatable/scable/skewable GBA hardware sprite.
		// The GBA can draw a max of 32 of these at a time.
		// See https://www.coranac.com/tonc/text/affobj.htm for more detail
		class ObjectAffine
		{
			u16 fill0[3];
			s16 m_pa;
			u16 fill1[3];
			s16 m_pb;
			u16 fill2[3];
			s16 m_pc;
			u16 fill3[3];
			s16 m_pd;

		public:
			ObjectAffine() : m_pa(0), m_pb(0), m_pc(0), m_pd(0) {}
			~ObjectAffine();

		} ALIGN(4);
	}
}
