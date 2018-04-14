#ifndef PRAGMA_ONCE_ENGINE_MATH_MATH_H
#define PRAGMA_ONCE_ENGINE_MATH_MATH_H

namespace Math
{
	template<typename T, typename G>
	inline T Min(T a, G b) {
		return (a < b) ? a : b;
	}

	template<typename T, typename G>
	inline T Max(T a, G b) {
		return (a > b) ? a : b;
	} 

	template<typename T>
	inline int Round(T x) {
		return (int)((x) + 0.5f);
	}
};

#endif
