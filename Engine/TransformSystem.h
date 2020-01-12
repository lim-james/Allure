#ifndef TRANSFORM_SYSTEM_H
#define TRANSFORM_SYSTEM_H

#include "System.h"
#include "TransformComparator.h"

#include <Containers/QuadTree.hpp>
#include <Events/Event.h>

class TransformSystem : public System {

	QuadTree<Transform*, TransformComparator> * tree;

public:

	~TransformSystem() override;

	void Initialize() override;
	void Update(const float& dt) override;

	void ActiveHanlder(Events::Event* event);

private:

	void SetTransform(Transform * const transform);

};

#endif
