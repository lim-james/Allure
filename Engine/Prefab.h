#ifndef PREFAB_H
#define PREFAB_H

#include "EntityManager.h"
#include "Transform.h"

#include <Encoding/Resource.h>

struct Prefab : Resource {

	void Initialize(EntityManager* const manager);
	virtual Transform* Create() = 0;

protected:

	EntityManager* entities;

};

#endif
