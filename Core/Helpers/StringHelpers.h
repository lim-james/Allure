#ifndef STRING_HELPERS_H
#define STRING_HELPERS_H

#include <vector>
#include <map>
#include <string>
#include <sstream>

namespace Helpers {

	template<typename T>
	T Parse(const std::string& content);
	template<typename T>
	std::string ToString(const T& content);

	std::vector<std::string> Split(const std::string& content, const char& delimiter);

	template<typename T>
	std::vector<T> Split(const std::string& content, const char& delimiter);

	std::string Join(const std::vector<std::string>& list);
	std::string Join(const std::vector<std::string>& list, const char& separator);
	std::string Join(const std::vector<std::string>& list, const std::string& separator);

	template<typename Separator>
	std::pair<std::string, std::string> Pair(const std::string& content, const Separator& separator);
	template<typename Value, typename Separator>
	std::pair<std::string, Value> Pair(const std::string& content, const Separator& separator);
	template<typename Key, typename Value, typename Separator>
	std::pair<Key, Value> Pair(const std::string& content, const Separator& separator);
	
	template<typename Separator>
	std::map<std::string, std::string> GetDictionary(const std::string& content, const char& delimiter, const Separator& separator);
	template<typename Value, typename Separator>
	std::map<std::string, Value> GetDictionary(const std::string& content, const char& delimiter, const Separator& separator);
	template<typename Key, typename Value, typename Separator>
	std::map<Key, Value> GetDictionary(const std::string& content, const char& delimiter, const Separator& separator);

	std::string Trim(std::string content);

}

#include "StringHelpers.inl"

#endif