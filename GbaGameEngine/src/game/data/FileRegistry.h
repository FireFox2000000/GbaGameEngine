#pragma once
#include "engine/io/File.h"
#include "engine/io/FileSystem.h"

/// <summary>
/// Holds a game-specific set of files and directories
/// </summary>
class FileRegistry : public IO::FileSystem::FileRegistry
{
public:
	FileRegistry();

	int TotalFiles() const override;
	const char* const* GetPaths() const override;
	const FilePtr* GetFiles() const override;
};
