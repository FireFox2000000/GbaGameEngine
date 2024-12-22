#pragma once
#include "engine/base/Typedefs.h"
#include "engine/base/core/stl/Span.h"
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

		T obj = *reinterpret_cast<const T*>(&fileData[m_streamPos.charIndex]);
		m_streamPos.charIndex += sizeof(T);

		return obj;
	}

	template<typename T>
	const T* ReadAddress(int size)
	{
		AdvanceToAlignment(sizeof(T));

		const T* obj = reinterpret_cast<const T*>(&fileData[m_streamPos.charIndex]);
		m_streamPos.charIndex += sizeof(T) * size;

		return obj;
	}

	template<typename T>
	Span<const T> ReadSpan(int size)
	{
		const T* data = ReadAddress<T>(size);
		return { data, static_cast<u32>(size) };
	}
};
