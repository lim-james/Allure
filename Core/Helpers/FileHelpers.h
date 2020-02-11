#ifndef FILE_HELPERS_H
#define FILE_HELPERS_H

#include <String>
#include <sys/stat.h>

namespace Helpers {

	struct stat GetFileInfo(const std::string& filepath);
	std::string GetFileName(const std::string& filepath);
	std::string ReadFile(const std::string& filepath);
	void WriteFile(const std::string& filepath, const std::string& content, const int& mode = std::ios::binary);

}

#endif
