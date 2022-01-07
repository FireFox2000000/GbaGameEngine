#pragma once
#include "engine/base/Typedefs.h"
#include "engine/base/Macros.h"

namespace Compression
{
	enum Type
	{
		// Maps to GBATek to make things a little easier - https://www.akkit.org/info/gbatek.htm#biosdecompressionfunctions
			// Maximum of 4 bits, or 16 values
		None = 0,
		LZ77 = 1,
		Huffman = 2,
		RunLength = 3,
		DiffFiltered = 8,
		BitPacked = 9,
	};

	static const int COMPRESSED_TYPE_BIT_INDEX = 4;
	static const int COMPRESSED_TYPE_BIT_SIZE = 4;

	Compression::Type GetCompressionType(u32 compressionFlags);

	// Decompression algorithms
	u8 GetBitPackedSrcBpp(u32 compressionFlags);
	void BitUnpack(void *dstv, const void *srcv, u32 srcLen, int srcBpp, int destBpp, u32 base = 0);

	// Regular bitfields may do assignments in bytes, which is very slow when copying to VRAM/OAM and may be corrupted. Use this instead in those cases.
#define	SafeBitFieldSet(Dest, Value, BitIndex, BitLength)\
	{\
		const u32 mask = BITS_INDEXED_U32((BitLength), (BitIndex));\
		(Dest) = ((Dest) & ~mask) | (((Value) << (BitIndex)) & mask);\
	}

#define BitFieldExtract(Src, BitIndex, BitLength) ((Src) & BITS_INDEXED_U32((BitLength), (BitIndex))) 
}
