#include "AENotation.hpp"

AENotation::AENotation(const std::string & encoded) {
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

std::string AENotation::Get(const std::string& key) const {
	if (values.find(key) == values.end()) {
		Debug::Error << "Key \"" << key << "\" not found in data set!";
		return "";
	}

	return values.at(key);
}

void AENotation::Set(const std::string& key, const std::string& value) {
	keyOrder.push_back(key);
	values[key] = value;
}


