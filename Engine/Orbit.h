#ifndef ORBIT_H
#define ORBIT_H

#include "Script.h"

struct Orbit : Script {

	float radius;
	Transform* orbit;

private:

	float t;

	void Awake() override;
	void Update() override;

};

#endif
