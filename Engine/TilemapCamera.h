#ifndef	TILEMAP_CAMERA_H
#define TILEMAP_CAMERA_H

#include "Script.h"
#include "Camera.h"
#include "TilemapRender.h"

struct TilemapCamera : Script {

	using base_type = TilemapCamera;

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
