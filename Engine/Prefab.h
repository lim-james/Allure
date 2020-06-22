#ifndef PREFAB_H
#define PREFAB_H

#include "EntityManager.h"
#include "Transform.h"

#include <Encoding/Resource.h>

struct Prefab : Resource {

	void Initialize(EntityManager* const manager);
	virtual Transform* Create() = 0;
	Transform* CreateIn(Transform* const parent);
	Transform* CreateIn(Transform* const parent, vec3f const& position);
	Transform* CreateAt(vec3f const& position);

protected:

	EntityManager* entities;

};

#endif
