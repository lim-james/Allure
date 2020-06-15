#include "ColorHelpers.h"

#include "../Math/Converter.h"

vec4f Helpers::GetColor(std::string const & hex) {
	static const float inv = 1.f / 255.f;

	vec4f result(1.f);

	const unsigned r = Math::HexToDec(hex.substr(0, 2));
	const unsigned g = Math::HexToDec(hex.substr(2, 2));
	const unsigned b = Math::HexToDec(hex.substr(4, 2));

	result.r = static_cast<float>(r) * inv;
	result.g = static_cast<float>(g) * inv;
	result.b = static_cast<float>(b) * inv;

	return result;
}
