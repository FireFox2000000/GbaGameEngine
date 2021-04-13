#include "Math.h"

int Math::Mod(int x, unsigned int N)
{
	return (x % N + N) % N;
}
