#pragma once
#include "engine/base/Typedefs.h"

class MemoryMappedFileView
{
	const u32* m_data = nullptr;

public:
	MemoryMappedFileView() = default;
	MemoryMappedFileView(const u32* const data) : m_data(data) {};
	const u32* GetData() const { return m_data; }

	operator bool() const { return m_data; }
};
