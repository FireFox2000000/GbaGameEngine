#pragma once

#define UNUSED(x) ((void)(x))		// Disable compiler warnings on unused variables
#include "core\Nullptr.h"

// Data alignment
#define ALIGN(n) __attribute__((aligned(n)))
#define PACKED __attribute__((packed))

#define BIT(index) (1 << index)
#define SHIFTED_BITMASK(bits, index) (bits << index)	// Shift a full bitmask along to a specific index
#define BITS_U32(length) (0xFFFFFFFF >> (32 - (length)))	// Create a mask full of 1's for a specific size
#define BITS_INDEXED_U32(length, index) (SHIFTED_BITMASK(BITS_U32(length), index))	// Create a series of enabled bits starting at a specific index for a specific size

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define ROUND(x) ((int)((x) + 0.5f))

#define NULL nullptr
