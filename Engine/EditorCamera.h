#ifndef EDITOR_CAMERA_H
#define EDITOR_CAMERA_H

#include "Script.h"
#include "Camera.h"

struct EditorCamera : Script {

	float speed;

private:

	Camera* camera;

	bool controlling;
	vec3f direction;
	vec2f cursorPosition;

	void Awake() override;
	void Update() override;

	void KeyHandler(Events::Event * event);
	void MouseButtonHandler(Events::Event * event);
	void CursorPositionHandler(Events::Event * event);

	vec3f ScreenToRay(vec2f const& screenPosition);

};

#endif
