#include "FileHelpers.h"

#include "../Logger/Logger.h"
#include "StringHelpers.h"	

#include <fstream>
#include <streambuf>

struct stat Helpers::GetFileInfo(std::string const& filepath) {
	struct stat fileInfo;
	stat(filepath.c_str(), &fileInfo);
	return fileInfo;
}

std::string Helpers::GetFileName(std::string const& filepath) {
	return Helpers::Split(filepath, '\\').back();
}

std::string Helpers::ReadFile(std::string const& filepath) {
	std::ifstream ifs(filepath);

	if (!ifs.is_open()) {
		Debug::Warn << filepath << " not valid.\n";
		return "";
	}

	const std::string result(
		(std::istreambuf_iterator<char>(ifs)),
		std::istreambuf_iterator<char>()
	);
	ifs.close();

	return result;
}


void Helpers::WriteFile(std::string const& filepath, std::string const& content, int const& mode) {
	std::ofstream ofs(filepath, mode);
	ofs << content;
}

