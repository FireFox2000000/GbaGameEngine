#pragma once
#include "engine/base/Typedefs.h"

class CppFileReader
{
	struct StreamPos
	{
		int charIndex = 0;
	};

	StreamPos m_streamPos;

	void AdvanceToAlignment(int alignmentSize) {
		DEBUG_ASSERTMSG((alignmentSize & (alignmentSize - 1)) == 0, "alignment must be power of 2");

		int remainder = m_streamPos.charIndex & (alignmentSize - 1);
		if (remainder > 0)	
		{
			// unaligned
			m_streamPos.charIndex += alignmentSize - remainder;
		}
	}

public:
	using FilePtr = const u32*;
	const u8* fileData = nullptr;

	CppFileReader(FilePtr file)
	{
		fileData = (u8*)file;
	}

	template<typename T>
	T Read()
	{
		AdvanceToAlignment(sizeof(T));

		T obj = *(T*)(&fileData[m_streamPos.charIndex]);
		m_streamPos.charIndex += sizeof(T);

		return obj;
	}

	template<typename T>
	T* ReadAddress(int size)
	{
		AdvanceToAlignment(sizeof(T));

		T* obj = (T*)(&fileData[m_streamPos.charIndex]);
		m_streamPos.charIndex += sizeof(T) * size;

		return obj;
	}
};
