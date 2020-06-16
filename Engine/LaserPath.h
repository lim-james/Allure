#ifndef LASER_PATH_H
#define LASER_PATH_H

#include "Prefab.h"

struct LaserPath : Prefab {
	Transform* Create() override;
};

#endif
