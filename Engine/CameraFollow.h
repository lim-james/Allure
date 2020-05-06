#ifndef CAMERA_FOLLOW_H
#define CAMERA_FOLLOW_H

#include "Script.h"

#include <Math/Vectors.h>

struct CameraFollow : Script {
	
	Transform* target;

	vec3f offset;

	bool stick;
	float speed;

private:

	void Update() override;

};


#endif
