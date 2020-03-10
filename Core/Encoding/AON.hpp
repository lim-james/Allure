#ifndef ALLURE_OBJECT_NOTATION_HPP
#define ALLURE_OBJECT_NOTATION_HPP

#include "../Helpers/StringHelpers.h"
#include "../Logger/Logger.h"

#include <map>
#include <string>

struct AON {

	template<typename T>
	T Get(const std::string& key);

	template<typename T>
	void Set(const std::string& key, const T& value);

private:

	std::map<std::string, std::string> values;

};

template<typename T>
T AON::Get(const std::string& key) {
	if (values.find(key) == values.end()) {
		Debug::Error << "Key \"" << key << "\" not found in data set!";
		return static_cast<T>(0);
	}

	return Helpers::Parse<T>(values[key]);
}

template<typename T>
void AON::Set(const std::string& key, const T& value) {
	values[key] = Helpers::ToString(value);
}

#endif
