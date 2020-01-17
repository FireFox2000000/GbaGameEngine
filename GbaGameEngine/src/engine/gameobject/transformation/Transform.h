#pragma once
#include "engine/base/core/stl/FixedPoint.h"
#include "engine/math/Vector2.h"

namespace Component
{
	struct Transform
	{
		using tPosition = Vector2 <tFixedPoint8>;
		using tScale = Vector2 <tFixedPoint8>;

	private:
		// Todo, replace with transformation matrix
		tPosition m_localPosition;
		tScale m_localScale = Vector2<tFixedPoint8>(1, 1);

	public:
		inline tPosition GetLocalPosition() const { return m_localPosition; }
		inline void SetLocalPosition(tPosition pos) { m_localPosition = pos; }
		template<typename T>
		inline void SetLocalPosition(T x, T y) { SetLocalPosition(tPosition(x, y)); }

		// World position, todo
		inline tPosition GetPosition() const { return GetLocalPosition(); }
		inline void SetPosition(tPosition pos) { SetLocalPosition(pos); }
		template<typename T>
		inline void SetPosition(T x, T y) { SetPosition(tPosition(x, y)); }

		inline tScale GetLocalScale() const { return m_localScale; }
		inline void SetLocalScale(tScale scale) { m_localScale = scale; }

		// World scale, todo
		inline tScale GetScale() const { return GetLocalScale(); }
		inline void SetScale(tScale scale) { SetLocalScale(scale); }
	};
}
