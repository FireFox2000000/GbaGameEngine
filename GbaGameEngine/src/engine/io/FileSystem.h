#pragma once
#include "File.h"

namespace IO
{

	class FileSystem
	{
	public:
		class FileRegistry
		{
		public:
			virtual int TotalFiles() const = 0;
			virtual const char* const* GetPaths() const = 0;
			virtual const FilePtr* GetFiles() const = 0;
		};

	private:
		FileRegistry* m_fileRegistry = nullptr;

	public:
		void SetRegistry(FileRegistry* fileRegistry);

		/// <summary>
		/// Read only at the moment
		/// </summary>
		/// <param name="path"></param>
		/// <returns></returns>
		FilePtr Open(const char* path);
	};
}
