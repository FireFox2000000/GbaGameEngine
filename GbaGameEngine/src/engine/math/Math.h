#ifndef PRAGMA_ONCE_ENGINE_MATH_MATH_H
#define PRAGMA_ONCE_ENGINE_MATH_MATH_H
#include <cmath>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define ROUND(x) (static_cast<int>((x) + 0.5f))

#define SECONDS_TO_MILLISECONDS(x) ((x) * 1000)
#define MILLISECONDS_TO_SECONDS(x) ((x) / 1000)

namespace Math
{
	template<typename T>
	inline bool SignCompare(T a, T b) {
		return a * b > 0;
	}
}

#endif
