#ifndef ALLURE_ENCODED_NOTATION_HPP
#define ALLURE_ENCODED_NOTATION_HPP

#include "../Helpers/StringHelpers.h"
#include "../Logger/Logger.h"

#include <unordered_map>
#include <string>

struct AENotation {

	AENotation() = default;
	AENotation(std::string const& encoded);

	void Reset();
	std::string ToString() const;

	std::string Get(std::string const& key) const;
	template<typename T>
	T Get(std::string const& key) const;

	void Set(std::string const& key, std::string const& value);
	template<typename T>
	void Set(std::string const& key, T const& value);

private:

	std::vector<std::string> keyOrder;
	std::unordered_map<std::string, std::string> values;

};

template<typename T>
T AENotation::Get(std::string const& key) const {
	if (values.find(key) == values.end()) {
		Debug::Error << "Key \"" << key << "\" not found in data set!\n";
		return static_cast<T>(0);
	}

	return Helpers::Parse<T>(values.at(key));
}

template<typename T>
void AENotation::Set(std::string const& key, T const& value) {
	//values.insert(key, );
	keyOrder.push_back(key);
	values[key] = Helpers::ToString(value);
}

#endif
