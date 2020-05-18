#include "StringHelpers.h"

#include <iterator>

std::vector<std::string> Helpers::Split(std::string const& content, char const& delimiter) {
	std::vector<std::string> parts;
	
	std::string buffer;
	std::istringstream iss(content);

	while (std::getline(iss, buffer, delimiter))
		parts.push_back(buffer);

	return parts;
}

std::string Helpers::Join(std::vector<std::string> const& list) {
	std::string result;

	for (auto const& item : list)
		result += item;

	return result;
}

std::string Helpers::Join(std::vector<std::string> const& list, char const& separator) {
	std::string result;

	for (auto const& item : list)
		result += item + separator;

	return result;
}

std::string Helpers::Join(std::vector<std::string> const& list, std::string const& separator) {
	std::string result;

	for (auto const& item : list)
		result += item + separator;

	return result;
}

//std::pair<std::string, std::string> Helpers::Pair(std::string const& content, char const& separator) {
//	const auto position = content.find(separator);
//	return std::make_pair(content.substr(0, position), content.substr(position + 1));
//}
//
//std::pair<std::string, std::string> Helpers::Pair(std::string const& content, std::string const& separator) {
//	const auto position = content.find(separator);
//	return std::make_pair(content.substr(0, position), content.substr(position + 1));
//}
//
//std::pair<std::string, std::string> Helpers::Pair(std::string const& content, const char* separator) {
//	const auto position = content.find(separator);
//	return std::make_pair(content.substr(0, position), content.substr(position + 1));
//}

std::string Helpers::Trim(std::string content) {
	int length = content.length();
	while (length > 0 && content[0] == ' ') content.erase(0, 1);
	while (length > 0 && content[length - 1] == ' ') content.erase(--length, 1);

	return content;
}

std::string Helpers::Pad(std::string const & content, unsigned const & length, char const & padding) {
	std::string result = content;

	while (result.length() < length) {
		result.insert(result.begin(), padding);
	}

	return result;
}
