#pragma once
#include "engine/base/Typedefs.h"

class CppFileReader
{
	struct StreamPos
	{
		int charIndex = 0;
		int lastAlignedSize = 0;
	};

	StreamPos m_streamPos;

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
		int alignment = sizeof(T) - m_streamPos.lastAlignedSize;
		if (alignment > 0 && m_streamPos.lastAlignedSize > 0)
		{
			m_streamPos.charIndex += alignment;
		}

		T obj = *(T*)(&fileData[m_streamPos.charIndex]);

		m_streamPos.charIndex += sizeof(T);
		m_streamPos.lastAlignedSize = sizeof(T);

		return obj;
	}

	template<typename T>
	T* ReadAddress(int size)
	{
		int alignment = sizeof(T) - m_streamPos.lastAlignedSize;
		if (alignment > 0 && m_streamPos.lastAlignedSize > 0)
		{
			m_streamPos.charIndex += alignment;
		}

		T* obj = (T*)(&fileData[m_streamPos.charIndex]);

		m_streamPos.charIndex += sizeof(T) * size;
		m_streamPos.lastAlignedSize = sizeof(T);

		return obj;
	}
};
