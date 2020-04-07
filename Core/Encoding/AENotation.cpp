#include "AENotation.hpp"

AENotation::AENotation(std::string const& encoded) {
	for (auto& pair : Helpers::GetDictionary(encoded, '\n', ':'))
		Set(pair.first, pair.second);
}

void AENotation::Reset() {
	keyOrder.clear();
	values.clear();
}

std::string AENotation::ToString() const {
	std::string result = "";

	for (auto& key : keyOrder)
		result += key + ":" + values.at(key) + '\n';

	return result;
}

std::string AENotation::Get(std::string const& key) const {
	if (values.find(key) == values.end()) {
		Debug::Error << "Key \"" << key << "\" not found in data set!";
		return "";
	}

	return values.at(key);
}

void AENotation::Set(std::string const& key, std::string const& value) {
	keyOrder.push_back(key);
	values[key] = value;
}


