#pragma once

namespace GBA
{
	struct VCount
	{
		char currentScanline : 8;
		char : 8;	// unused
	};

	// Read-only
	const volatile VCount* const ioRegisterVCount = reinterpret_cast<VCount*>(0x4000006);
}
