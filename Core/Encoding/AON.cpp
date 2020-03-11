#include "AON.hpp"

void AON::Reset() {
	values.clear();
}

std::string AON::ToString() const {
	std::string result = "";

	for (auto& pair : values)
		result += pair.first + ":" + pair.second + '\n';

	return result;
}
