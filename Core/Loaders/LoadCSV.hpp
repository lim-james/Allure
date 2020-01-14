#ifndef LOAD_CSV_HPP
#define LOAD_CSV_HPP

#include "../Helpers/StringHelpers.h"

#include <vector>
#include <string>
#include <fstream>

namespace Load {
	template<typename T>
	std::vector<std::vector<T>> CSV(const std::string& filepath);
}

template<typename T>
std::vector<std::vector<T>> Load::CSV(const std::string& filepath) {
	std::vector<std::vector<T>> result;

	std::ifstream ifs(filepath);

	std::string lineBuffer;
	while (!ifs.eof()) {
		std::getline(ifs, lineBuffer);
		if (lineBuffer[0] == '!') continue;

		result.push_back(Helpers::Split<T>(lineBuffer, ','));
	}

	return result;
}


#endif
