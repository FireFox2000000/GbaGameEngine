#pragma once
#include "engine/base/core/stl/FixedPoint.h"
#include "engine/math/Vector2.h"

namespace Component
{
	struct Transform
	{
	private:
		// Todo, replace with proper transformation matrix maybe
		Vector2<FPi16> m_localPosition;
		Vector2<FPi24> m_localScale = { 1, 1 };
		FPi8 m_localRotationDegrees = 0;
		bool m_hasAffineTrasformation = false;

		void UpdateHasAffineTransformation();

	public:
		Vector2<FPi16> GetLocalPosition() const;
		void SetLocalPosition(Vector2<FPi16> pos);
		inline void SetLocalPosition(FPi16 x, FPi16 y) { SetLocalPosition(Vector2<FPi16>(x, y)); }

		// World position, todo
		Vector2<FPi16> GetPosition() const;
		void SetPosition(Vector2<FPi16> pos);
		inline void SetPosition(FPi16 x, FPi16 y) { SetPosition(Vector2<FPi16>(x, y)); }

		Vector2<FPi24> GetLocalScale() const;
		void SetLocalScale(Vector2<FPi24> scale);

		// World scale, todo
		Vector2<FPi24> GetScale() const;
		void SetScale(Vector2<FPi24> scale);
		template<typename T>
		inline void SetScale(T x, T y) { SetScale(Vector2<FPi24>(x, y)); }

		FPi8 GetRotationDegrees() const;
		void SetRotationDegrees(FPi8 degrees);	

		bool HasAffineTransformation() const;
	};
}
