#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "System.h"
#include "Physics.h"
#include "PhysicsProperties.h"

#include <vector>

class PhysicsSystem : public System {

	std::vector<Physics*> components;

public:

	void Initialize() override;
	void Update(float const& dt) override;

private:
	
	void ActiveHandler(Events::Event* event);
	
};

#endif
