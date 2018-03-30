#pragma once

#define UNUSED(x) ((void)(x))		// Disable compiler warnings on unused variables

// Data alignment
#define ALIGN(n) __attribute__((aligned(n)))
#define PACKED __attribute__((packed))

#define BIT(index) (1 << index)
#define SHIFTED_BITMASK(bits, index) (bits << index)	// Shift a full bitmask along to a specific index

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define ROUND(x) ((int)((x) + 0.5f))