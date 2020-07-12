#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "EngineProperties.h"

#include "System.h"
#include "Physics.h"
#include "Transform.h"

#include <vector>
#include <map>

class PhysicsSystem : public System {

	struct Delta {
		vec3f translation;
		vec3f rotation;
	};

	float invFR; // inverse framerate
	std::vector<Physics*> components;
	std::map<Transform*, Delta> deltas;

public:

	void Initialize() override;
	void Update(float const& dt) override;
	void FixedUpdate(float const& dt) override;

private:
	
	void ActiveHandler(Events::Event* event);
	
};

#endif
