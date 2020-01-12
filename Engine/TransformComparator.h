#ifndef TRANSFORM_COMPARATOR_H
#define TRANSFORM_COMPARATOR_H

#include "Transform.h"

#include <Containers/Comparator.h>

struct TransformComparator : Comparator<Transform*> {

	void Partition(Quad<Transform*>* parent) override;

private:
	
	void Add(Transform* const item, Quad<Transform*>* const quad) const;

};

#endif
