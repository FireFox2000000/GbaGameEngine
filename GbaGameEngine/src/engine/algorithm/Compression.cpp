#include "Compression.h"
#include "engine/base/Macros.h"
#include "engine/base/BitTwiddling.h"
#include "engine/math/Math.h"

Compression::Type Compression::GetCompressionType(u32 compressionFlags)
{
	return Compression::Type(BITS_U32(Compression::COMPRESSED_TYPE_BIT_SIZE) & (compressionFlags >> Compression::COMPRESSED_TYPE_BIT_INDEX));
}

u8 Compression::GetBitPackedSrcBpp(u32 compressionFlags)
{
	return compressionFlags & BITS_U32(Compression::COMPRESSED_TYPE_BIT_INDEX);
}

constexpr u8 c_totalWrdBits = 32;
constexpr u8 c_baseBits = c_totalWrdBits - 1;
void Compression::BitUnpack(void * dstv, const void * srcv, u32 srcLen, int srcBpp, int destBpp, u32 base)
{	
	u32 *src = (u32*)srcv;
	u32 *dst = (u32*)dstv;

	u32 bBase0 = base & (1 << c_baseBits);    // add to 0 too?
	base &= ~(1 << c_baseBits);

	// if srcBpp == destBpp then skip over most of this and just add base if nessacary
	if (srcBpp == destBpp)
	{
		u32 length = srcLen >> 2;
		if (!bBase0)
		{
			for (u32 i = 0; i < length; ++i)
			{
				*dst++ = *(src++);
			}

			return;
			
		}

		// else NOT IMPLEMENTED, use regular. Implementation would be an optimised version, if possible
	}

	u32 srcBppFac = BitTwiddling::IndexOfHighestSetBit(srcBpp) + 2;
	u32 len = (srcLen * destBpp + 3) >> srcBppFac; // # dst words

	const u32 srcMask = BITS_U32(srcBpp);

	// https://www.coranac.com/tonc/text/text.htm
	// Modified to handle any srcBpp format rather than just 1bpp
	u32 swd, ssh = 32;    // src data and shift
	u32 dwd, dsh;       // dst data and shift
	while (len--)
	{
		if (ssh >= c_totalWrdBits)
		{
			swd = *src++;
			ssh = 0;
		}
		dwd = 0;
		for (dsh = 0; dsh < c_totalWrdBits; dsh += destBpp)
		{
			u32 wd = swd & srcMask;
			if (wd || bBase0)
				wd += base;
			dwd |= wd << dsh;
			swd >>= srcBpp;
			ssh += srcBpp;
		}

		*dst++ = dwd;
	}
}
