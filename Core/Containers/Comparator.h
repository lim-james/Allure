#ifndef COMPARATOR_H
#define COMPARATOR_H

#include "Quad.hpp"

template<typename ContentType>
struct Comparator {
	// partitions the quad passed in and returns needed
	virtual void Partition(Quad<ContentType>* parent) = 0;
};

#endif