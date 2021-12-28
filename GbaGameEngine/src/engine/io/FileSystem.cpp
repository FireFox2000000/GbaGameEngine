#include "FileSystem.h"
#include <cstring>
#include <algorithm>
#include "engine/base/Macros.h"

FilePtr IO::FileSystem::Open(const char* path)
{
	// Assume the paths are pre-sorted alphabetically and then binary search for the file. 
	auto paths = m_fileRegistry->GetPaths();
	auto files = m_fileRegistry->GetFiles();

	auto begin = &paths[0];
	auto end = &paths[m_fileRegistry->TotalFiles()];

	auto it = std::lower_bound(begin, end, path, [](const char* a, const char* b) { 
		return strcmp(a, b) < 0;
		});

	if (it != end && strcmp(*it, path) == 0)
	{
		std::size_t index = std::distance(begin, it);
		return files[index];
	}

	DEBUG_ERRORFORMAT("Unable to find file %s", path);

	return nullptr;
}

void IO::FileSystem::SetRegistry(FileRegistry* fileRegistry)
{
	m_fileRegistry = fileRegistry;
}
