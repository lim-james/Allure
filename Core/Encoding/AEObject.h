#ifndef ALLURE_ENCODED_OBJECT_H
#define ALLURE_ENCODED_OBJECT_H

#include "AENotation.hpp"
#include "Resource.h"

// Allure Encoded Object
struct AEObject: Resource {

	void Parse(const AENotation& data);
	std::string ToString();
	
protected:

	virtual void Unpack(const AENotation& data) = 0;
	virtual void Pack(AENotation& data) = 0;

};


#endif
