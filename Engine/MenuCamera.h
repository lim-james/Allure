#ifndef MENU_CAMERA_H
#define MENU_CAMERA_H

#include "Script.h"

#include <Events/Event.h>

struct MenuCamera : Script {

	using base_type = MenuCamera;

	float speed;
	vec2f range;

private:

	vec2f target;
	vec2f windowUnit;

	void Awake() override;
	void Start() override;
	void Update() override;

	void CursorPositionHandler(Events::Event* event);
	void WindowSizeHandler(Events::Event* event);

};

#endif
