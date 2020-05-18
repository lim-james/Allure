#ifndef STRING_HELPERS_H
#define STRING_HELPERS_H

#include <vector>
#include <map>
#include <string>
#include <sstream>

namespace Helpers {

	template<typename T>
	T Parse(std::string const& content);
	template<typename T>
	std::string ToString(T const& content);

	std::vector<std::string> Split(std::string const& content, char const& delimiter);

	template<typename T>
	std::vector<T> Split(std::string const& content, char const& delimiter);

	std::string Join(std::vector<std::string> const& list);
	std::string Join(std::vector<std::string> const& list, char const& separator);
	std::string Join(std::vector<std::string> const& list, std::string const& separator);

	template<typename Separator>
	std::pair<std::string, std::string> Pair(std::string const& content, Separator const& separator);
	template<typename Value, typename Separator>
	std::pair<std::string, Value> Pair(std::string const& content, Separator const& separator);
	template<typename Key, typename Value, typename Separator>
	std::pair<Key, Value> Pair(std::string const& content, Separator const& separator);
	
	template<typename Separator>
	std::map<std::string, std::string> GetDictionary(std::string const& content, char const& delimiter, Separator const& separator);
	template<typename Value, typename Separator>
	std::map<std::string, Value> GetDictionary(std::string const& content, char const& delimiter, Separator const& separator);
	template<typename Key, typename Value, typename Separator>
	std::map<Key, Value> GetDictionary(std::string const& content, char const& delimiter, Separator const& separator);

	std::string Trim(std::string content);
	std::string Pad(std::string const& content, unsigned const& length, char const& padding);

}

#include "StringHelpers.inl"

#endif