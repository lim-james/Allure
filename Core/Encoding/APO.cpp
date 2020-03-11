#include "APO.h"

void APO::Parse(const std::string & encoded) {
}

void APO::ToString() {
	data.Reset();
	Pack();

	Debug::Warn << '\n' << data.ToString() << '\n';
	//return data.ToString();
}
