#include "Converter.h"

#include <sstream>
#include <iomanip>

unsigned Math::HexToDec(std::string const & hex) {
	unsigned result;
	std::istringstream is(hex);
	is >> std::hex >> result;
	return result;
}
