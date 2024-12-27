#pragma once
#include "engine/base/Typedefs.h"
#include "engine/base/core/stl/Span.h"
#include "engine/io/MemoryMappedFileView.h"

class MemoryMappedFileStream
{
	struct StreamPos
	{
		int charIndex = 0;
	};

	StreamPos m_streamPos;
	const u8* fileData = nullptr;

	void AdvanceToAlignment(int alignmentSize);

	template<typename T>
	const T* ReadAddress(int size)
	{
		AdvanceToAlignment(sizeof(T));

		const T* obj = reinterpret_cast<const T*>(&fileData[m_streamPos.charIndex]);
		m_streamPos.charIndex += sizeof(T) * size;

		return obj;
	}

public:
	MemoryMappedFileStream(MemoryMappedFileView file);

	MemoryMappedFileView GetFileLocation() const { return reinterpret_cast<const u32*>(fileData); }

	template<typename T>
	T Read()
	{
		AdvanceToAlignment(sizeof(T));

		T obj = *reinterpret_cast<const T*>(&fileData[m_streamPos.charIndex]);
		m_streamPos.charIndex += sizeof(T);

		return obj;
	}

	template<typename T>
	Span<const T> Read(int size)
	{
		const T* data = ReadAddress<T>(size);
		return { data, static_cast<u32>(size) };
	}

	template<typename T>
	void operator >> (T& out_obj)
	{
		out_obj = Read<T>();
	}
};
