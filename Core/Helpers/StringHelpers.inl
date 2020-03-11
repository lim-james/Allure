#include "StringHelpers.h"

template<typename T>
T Helpers::Parse(const std::string& content) {
	std::istringstream iss(content);
	T result;
	iss >> result;
	return result;
}

template<typename T>
std::string Helpers::ToString(const T & content) {
	std::ostringstream oss;
	oss << content;
	return oss.str();
}

template<typename T>
std::vector<T> Helpers::Split(const std::string& content, const char& delimiter) {
	std::vector<T> parts;

	std::string buffer;
	std::istringstream iss(content);

	while (std::getline(iss, buffer, delimiter))
		parts.push_back(Parse<T>(buffer));

	return parts;
}

template<typename Separator>
std::pair<std::string, std::string> Helpers::Pair(const std::string& content, const Separator& separator) {
	const auto position = content.find(separator);
	return std::make_pair(content.substr(0, position), content.substr(position + 1));
}

template<typename Value, typename Separator>
std::pair<std::string, Value> Helpers::Pair(const std::string& content, const Separator& separator) {
	const auto position = content.find(separator);
	return std::make_pair(
		content.substr(0, position),
		Parse<Value>(content.substr(position + 1))
	);
}

template<typename Key, typename Value, typename Separator>
std::pair<Key, Value> Helpers::Pair(const std::string& content, const Separator& separator) {
	const auto position = content.find(separator);
	return std::make_pair(
		Parse<Key>(content.substr(0, position)),
		Parse<Value>(content.substr(position + 1))
	);
}

template<typename Separator>
std::map<std::string, std::string> Helpers::GetDictionary(const std::string& content, const char& delimiter, const Separator& separator) {
	std::map<std::string, std::string> result;
	const auto list = Split(content, delimiter);

	for (const auto& item : list)
		result.insert(Pair(item, separator));

	return result;
}

template<typename Value, typename Separator>
std::map<std::string, Value> Helpers::GetDictionary(const std::string& content, const char& delimiter, const Separator& separator) {
	std::map<std::string, Value> result;
	const auto list = Split(content, delimiter);

	for (const auto& item : list)
		result.insert(Pair<Value>(item, separator));

	return result;
}

template<typename Key, typename Value, typename Separator>
std::map<Key, Value> Helpers::GetDictionary(const std::string& content, const char& delimiter, const Separator& separator) {
	std::map<Key, Value> result;
	const auto list = Split(content, delimiter);

	for (const auto& item : list)
		result.insert(Pair<Key, Value>(item, separator));

	return result;
}

