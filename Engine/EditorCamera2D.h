#ifndef EDITOR_CAMERA_2D_H
#define EDITOR_CAMERA_2D_H

#include "Script.h"
#include "Camera.h"

struct EditorCamera2D : Script {

	using base_type = EditorCamera2D;

	float minZoom;
	float maxZoom;

private:

	bool isPanning;

	Camera* camera;

	void Awake() override;
	void Start() override;

	void CursorPositionHandler(Events::Event* event);
	void MouseButtonHandler(Events::Event* event);
	void ScrollHandler(Events::Event* event);

	void StartPanning();
	void StopPanning();

};
	
#endif
