#include "MemoryMappedFileStream.h"
#include "engine/base/BitTwiddling.h"
#include "engine/base/Macros.h"

void MemoryMappedFileStream::AdvanceToAlignment(int alignmentSize)
{
	DEBUG_ASSERTMSG(BitTwiddling::IsPowerOf2(alignmentSize), "alignment must be power of 2");

	int remainder = m_streamPos.charIndex & (alignmentSize - 1);
	if (remainder > 0)
	{
		// unaligned
		m_streamPos.charIndex += alignmentSize - remainder;
	}
}

MemoryMappedFileStream::MemoryMappedFileStream(MemoryMappedFileView file)
{
	fileData = reinterpret_cast<const u8*>(file.GetData());
}
