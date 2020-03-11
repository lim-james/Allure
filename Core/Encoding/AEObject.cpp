#include "AEObject.h"

#include "../Helpers/StringHelpers.h"

void AEObject::Parse(const AENotation & data) {
	Unpack(data);
}

std::string AEObject::ToString() {
	AENotation data; Pack(data);
	return data.ToString() + ">>>\n";
}
