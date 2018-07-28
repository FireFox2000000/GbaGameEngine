#ifndef PRAGMA_ONCE_ENGINE_MATH_MATH_H
#define PRAGMA_ONCE_ENGINE_MATH_MATH_H
#include <cmath>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define ROUND(x) (static_cast<int>((x) + 0.5f))

namespace Math
{
	template<typename T>
	inline bool SignCompare(T a, T b) {
		return a * b > 0;
	}
}

#endif
