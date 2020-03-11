#ifndef ALLURE_PARSABLE_OBJECT_H
#define ALLURE_PARSABLE_OBJECT_H

#include "AON.hpp"

// Allure Parsable Object
struct APO {

	void Parse(const std::string& encoded);
	void ToString();
	
protected:

	virtual void Unpack() = 0;
	virtual void Pack() = 0;
	
	AON data;

};


#endif
