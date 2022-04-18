#pragma once
#include "engine/base/Typedefs.h"
#include "engine/io/File.h"

class CppFileReader
{
	struct StreamPos
	{
		int charIndex = 0;
	};

	StreamPos m_streamPos;
	const u8* fileData = nullptr;

	void AdvanceToAlignment(int alignmentSize);

public:
	CppFileReader(FilePtr file);

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

	void ResetStreamPos()
	{
		m_streamPos = StreamPos();
	}
};
