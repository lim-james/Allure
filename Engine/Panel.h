#ifndef PANEL_H
#define PANEL_H

#include "Script.h"
#include "Camera.h"

struct Panel : Script {

	Camera* camera;
	bool draggable;
	
private:

	bool isDragging;

	vec2f prevPosition;

	void Awake() override;
	void Update() override;

	void MouseButtonHandler(Events::Event * event);
	void CursorPositionHandler(Events::Event * event);

};

#endif
