#ifndef EDITOR_CAMERA_H
#define EDITOR_CAMERA_H

#include "InputEvents.h"
#include "Script.h"

struct EditorCamera : Script {

	float speed;

private:

	bool controlling;
	vec3f direction;

	void Awake() override;
	void Update() override;

	void KeyHandler(Events::Event * event);
	void MouseButtonHandler(Events::Event * event);
	void CursorPositionHandler(Events::Event * event);

};

#endif
