#pragma once
#include "engine/base/core/stl/FixedPoint.h"
#include "engine/math/Vector2.h"

namespace Component
{
	struct Transform
	{
		using tPosition = Vector2 <tFixedPoint8>;
		using tScale = Vector2 <tFixedPoint24>;

	private:
		// Todo, replace with proper transformation matrix maybe
		tPosition m_localPosition;
		tScale m_localScale = { 1, 1 };
		tFixedPoint8 m_localRotationDegrees = 0;
		bool m_hasAffineTrasformation = false;

		void UpdateHasAffineTransformation();

	public:
		tPosition GetLocalPosition() const;
		void SetLocalPosition(tPosition pos);
		template<typename T>
		inline void SetLocalPosition(T x, T y) { SetLocalPosition(tPosition(x, y)); }

		// World position, todo
		tPosition GetPosition() const;
		void SetPosition(tPosition pos);
		template<typename T>
		inline void SetPosition(T x, T y) { SetPosition(tPosition(x, y)); }

		tScale GetLocalScale() const;
		void SetLocalScale(tScale scale);

		// World scale, todo
		tScale GetScale() const;
		void SetScale(tScale scale);
		template<typename T>
		inline void SetScale(T x, T y) { SetScale(tScale(x, y)); }

		tFixedPoint8 GetRotationDegrees() const;
		void SetRotationDegrees(tFixedPoint8 degrees);	

		bool HasAffineTransformation() const;
	};
}
