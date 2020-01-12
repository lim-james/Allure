#include "FileHelpers.h"

#include "../Logger/Logger.h"
#include "StringHelpers.h"	

#include <fstream>
#include <streambuf>

std::string Helpers::GetFileName(const std::string & filepath) {
	return Helpers::Split(filepath, '\\').back();
}

std::string Helpers::ReadFile(const std::string& filepath) {
	std::ifstream ifs(filepath);

	if (!ifs.is_open()) {
		Console::Warn << filepath << " not valid.\n";
		return "";
	}

	const std::string result(
		(std::istreambuf_iterator<char>(ifs)),
		std::istreambuf_iterator<char>()
	);
	ifs.close();

	return result;
}


void Helpers::WriteFile(const std::string& filepath, const std::string& content, const int& mode) {
	std::ofstream ofs(filepath, mode);
	ofs << content;
}

