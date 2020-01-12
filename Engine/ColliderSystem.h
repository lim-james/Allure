#ifndef COLLIDER_SYSTEM_H
#define COLLIDER_SYSTEM_H

#include "System.h"
#include "Collider.h"

#include <Events/Event.h>

#include <map>
#include <vector>

class ColliderSystem : public System {

	std::map<unsigned, std::map<unsigned, bool>> history;
	std::vector<Collider*> components;

public:

	~ColliderSystem() override;

	void Initialize() override;
	void Update(const float& dt) override;

private:

	void ActiveHandler(Events::Event* event);

	void PerformAction(const unsigned& index, Collider * const c1, Collider * const c2);

};

#endif