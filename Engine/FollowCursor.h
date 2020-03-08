#ifndef FOLLOW_CURSOR_H
#define FOLLOW_CURSOR_H

#include "Script.h"

#include "Camera.h"

struct FollowCursor : Script {

	Camera* camera;
	
private:

	void Awake() override;

	// event handler
	void CursorPositionHandler(Events::Event* event);

};

#endif