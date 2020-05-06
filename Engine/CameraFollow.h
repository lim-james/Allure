#ifndef CAMERA_FOLLOW_H
#define CAMERA_FOLLOW_H

#include "Script.h"

#include <Math/Vectors.h>

struct CameraFollow : Script {
	
	Transform* player;
	Transform* crosshair;

	vec3f offset;
	float jutDistance;

	bool stick;
	float speed;

private:

	void Update() override;

};


#endif
