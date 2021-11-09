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
		// Todo, replace with proper transformation matrix maybe
		tPosition m_localPosition;
		tScale m_localScale = Vector2<tFixedPoint8>(1, 1);
		tFixedPoint8 m_localRotationDegrees = 0;
		bool m_hasAffineTrasformation = false;

		void UpdateHasAffineTransformation();

	public:
		inline tPosition GetLocalPosition() const { return m_localPosition; }
		inline void SetLocalPosition(tPosition pos) { m_localPosition = pos; }
		template<typename T>
		inline void SetLocalPosition(T x, T y) { SetLocalPosition(tPosition(x, y)); }

		// World position, todo
		inline tPosition GetPosition() const { return m_localPosition; }
		inline void SetPosition(tPosition pos) { m_localPosition = pos; }
		template<typename T>
		inline void SetPosition(T x, T y) { SetPosition(tPosition(x, y)); }

		inline tScale GetLocalScale() const { return m_localScale; }
		inline void SetLocalScale(tScale scale) { m_localScale = scale; UpdateHasAffineTransformation(); }

		// World scale, todo
		inline tScale GetScale() const { return GetLocalScale(); }
		inline void SetScale(tScale scale) { SetLocalScale(scale); }
		template<typename T>
		inline void SetScale(T x, T y) { SetScale(tScale(x, y)); }

		inline tFixedPoint8 GetRotationDegrees() const { return m_localRotationDegrees; }
		void SetRotationDegrees(tFixedPoint8 degrees);	

		bool HasAffineTransformation() const { return m_hasAffineTrasformation; }
	};
}
