#ifndef EDITOR_CAMERA_2D_H
#define EDITOR_CAMERA_2D_H

#include "Script.h"
#include "Camera.h"
#include "TilemapRender.h"

struct EditorCamera2D : Script {

	using base_type = EditorCamera2D;

	Camera* camera;
	Transform* cursor;

	bool enabled;

private:

	bool isPanning;
	vec3f cursorPosition;

	void Awake() override;
	void Start() override;
	void Update() override;

	void CursorPositionHandler(Events::Event* event);
	void MouseButtonHandler(Events::Event* event);
	void ScrollHandler(Events::Event* event);

	void StartPanning();
	void StopPanning();

};

#endif
