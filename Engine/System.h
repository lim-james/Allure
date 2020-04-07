#ifndef SYSTEM_H
#define SYSTEM_H

#include "EntityManager.h"

class System {

protected:

	EntityManager * entities;

public:

	virtual ~System();

	virtual void Initialize() = 0;
	virtual void Update(float const& dt) = 0;

	friend class SystemManager;

};

#endif
