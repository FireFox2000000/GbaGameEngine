#include "FileRegistry.h"
#include "GameFiles.h"
#include "engine/base/core/stl/Array.h"

// Provides 2 lists, one for all the filepaths/string keys to each file. 
// The other a pointer to the start of each file. 
// SOA format, maps path to file 1-1

constexpr int GetFileCount()
{
	int totalFiles = 0;

#define FILE(directory, filename) totalFiles += 1;
	ALL_FILES()
#undef FILE

	return totalFiles;
}

#define TOSTRING(a) #a

const Array<const char*, GetFileCount()> paths = {

#define FILE(directory, filename) TOSTRING(directory/filename),
	ALL_FILES()
#undef FILE

};

const Array<FilePtr, GetFileCount()> files = {
#define FILE(directory, filename) filename::data,
	ALL_FILES()
#undef FILE
};

FileRegistry::FileRegistry()
{
}

int FileRegistry::TotalFiles() const
{
	return GetFileCount();
}

const char* const* FileRegistry::GetPaths() const
{
	return paths.begin();
}

const FilePtr* FileRegistry::GetFiles() const
{
	return files.begin();
}
