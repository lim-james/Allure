#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "System.h"
#include "Physics.h"

#include <Events/Event.h>

#include <map>
#include <vector>

class PhysicsSystem : public System {

	vec3f gravity;

	std::vector<Physics*> components;

public:

	~PhysicsSystem() override;

	void Initialize() override;
	void Update(float const& dt) override;

private:

	void ActiveHandler(Events::Event* event);

};

#endif
