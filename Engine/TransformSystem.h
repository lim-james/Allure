#ifndef TRANSFORM_SYSTEM_H
#define TRANSFORM_SYSTEM_H

#include "System.h"
#include "TransformComparator.h"

#include <Containers/QuadTree.hpp>
#include <Events/Event.h>

class TransformSystem : public System {

	bool updateStatic;
	QuadTree<Transform*, TransformComparator> * dynamicTree;
	QuadTree<Transform*, TransformComparator> * staticTree;

public:

	~TransformSystem() override;

	void Initialize() override;
	void Update(float const& dt) override;

private:

	void ActiveHanlder(Events::Event* event);
	void DynamicHanlder(Events::Event* event);

	void UpdateTree(QuadTree<Transform*, TransformComparator> const* tree);

};

#endif
