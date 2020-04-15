#ifndef FPS_CAMERA_H
#define FPS_CAMERA_H

#include "InputEvents.h"
#include "Script.h"

struct FPSCamera : Script {

	float speed;

private:

	void Awake() override;

	void KeyHandler(Events::Event * event);
	void CursorPositionHandler(Events::Event * event);

};

#endif
