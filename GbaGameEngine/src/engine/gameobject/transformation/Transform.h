#pragma once
#include "engine/base/core/stl/FixedPoint.h"
#include "engine/math/Vector2.h"

namespace Component
{
	class Transform
	{
	private:
		Vector2<FPi16> m_localPosition;
		Vector2<FPi16> m_localScale = { 1, 1 };
		FPi16 m_localRotationDegrees = 0;

	public:
		Vector2<FPi16> GetLocalPosition() const;
		void SetLocalPosition(Vector2<FPi16> pos);
		inline void SetLocalPosition(FPi16 x, FPi16 y) { SetLocalPosition(Vector2<FPi16>(x, y)); }

		// World position, todo
		Vector2<FPi16> GetPosition() const;
		void SetPosition(Vector2<FPi16> pos);
		inline void SetPosition(FPi16 x, FPi16 y) { SetPosition(Vector2<FPi16>(x, y)); }

		Vector2<FPi16> GetLocalScale() const;
		void SetLocalScale(Vector2<FPi16> scale);

		// World scale, todo
		Vector2<FPi16> GetScale() const;
		void SetScale(Vector2<FPi16> scale);
		inline void SetScale(FPi16 x, FPi16 y) { SetScale(Vector2<FPi16>(x, y)); }

		FPi16 GetRotationDegrees() const;
		void SetRotationDegrees(FPi16 degrees);

		// Math functions
		template <typename T>
		constexpr static bool HasAffineTransformation(const Vector2<T>& position, const Vector2<T>& scale, T rotationDegrees)
		{
			return rotationDegrees != 0 || (scale.x != 1 && scale.x != -1) || (scale.y != 1 && scale.y != -1);
		}
	};
}
