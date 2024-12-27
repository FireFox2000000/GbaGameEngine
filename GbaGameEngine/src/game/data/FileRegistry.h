#pragma once
#include "engine/io/FileSystem.h"

class MemoryMappedFileView;

/// <summary>
/// Holds a game-specific set of files and directories
/// </summary>
class FileRegistry : public IO::FileSystem::FileRegistry
{
public:
	FileRegistry();

	int TotalFiles() const override;
	const char* const* GetPaths() const override;
	const MemoryMappedFileView* GetFiles() const override;
};
