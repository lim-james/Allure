#ifndef ALLURE_ENCODED_NOTATION_HPP
#define ALLURE_ENCODED_NOTATION_HPP

#include "../Helpers/StringHelpers.h"
#include "../Logger/Logger.h"

#include <unordered_map>
#include <string>

struct AENotation {

	AENotation() = default;
	AENotation(const std::string& encoded);

	void Reset();
	std::string ToString() const;

	std::string Get(const std::string& key) const;
	template<typename T>
	T Get(const std::string& key) const;

	void Set(const std::string& key, const std::string& value);
	template<typename T>
	void Set(const std::string& key, const T& value);

private:

	std::vector<std::string> keyOrder;
	std::unordered_map<std::string, std::string> values;

};

template<typename T>
T AENotation::Get(const std::string& key) const {
	if (values.find(key) == values.end()) {
		Debug::Error << "Key \"" << key << "\" not found in data set!\n";
		return static_cast<T>(0);
	}

	return Helpers::Parse<T>(values.at(key));
}

template<typename T>
void AENotation::Set(const std::string& key, const T& value) {
	//values.insert(key, );
	keyOrder.push_back(key);
	values[key] = Helpers::ToString(value);
}

#endif
