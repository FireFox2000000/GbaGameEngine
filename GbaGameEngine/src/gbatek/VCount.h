#pragma once

namespace GBATEK
{
	struct VCount
	{
		char currentScanline : 8;
		char : 8;	// unused
	};

	static_assert(sizeof(VCount) == 2, "VCount struct malformed");

	// Read-only
	const volatile VCount* const ioRegisterVCount = reinterpret_cast<VCount*>(0x4000006);
}
