#ifndef FILE_HELPERS_H
#define FILE_HELPERS_H

#include <String>
#include <sys/stat.h>

namespace Helpers {

	struct stat GetFileInfo(std::string const& filepath);
	std::string GetFileName(std::string const& filepath);
	std::string ReadFile(std::string const& filepath);
	void WriteFile(std::string const& filepath, std::string const& content, int const& mode = std::ios::binary);

}

#endif
