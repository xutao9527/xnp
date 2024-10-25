#include "ShellFileInterface.h"
#include <cstdio>
#include <utility>

ShellFileInterface::ShellFileInterface(Rml::String  root) : root(std::move(root)) {}

ShellFileInterface::~ShellFileInterface() = default;

Rml::FileHandle ShellFileInterface::Open(const Rml::String& path)
{

	FILE* fp = fopen((root + path).c_str(), "rb");
	if (fp != nullptr)
		return (Rml::FileHandle)fp;
	fp = fopen(path.c_str(), "rb");
	return (Rml::FileHandle)fp;
}

void ShellFileInterface::Close(Rml::FileHandle file)
{
	fclose((FILE*)file);
}

size_t ShellFileInterface::Read(void* buffer, size_t size, Rml::FileHandle file)
{
	return fread(buffer, 1, size, (FILE*)file);
}

bool ShellFileInterface::Seek(Rml::FileHandle file, long offset, int origin)
{
	return fseek((FILE*)file, offset, origin) == 0;
}

size_t ShellFileInterface::Tell(Rml::FileHandle file)
{
	return ftell((FILE*)file);
}
