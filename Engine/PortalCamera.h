#ifndef PORTAL_CAMERA_H
#define PORTAL_CAMERA_H

#include "Script.h"
#include "Camera.h"

struct PortalCamera : Script {

	Transform* mainCamera;

	Transform* portal1;
	Transform* portal2;

private:

	void Update() override;

};

#endif
