#include "Math.h"

int Math::Mod(int x, int N)
{
	return (x % N + N) % N;
}
