#ifndef CROSSHAIR_CONTROLLER_H
#define CROSSHAIR_CONTROLLER_H

#include "Script.h"
#include "Camera.h"
#include "TransformAnimator.h"

struct CrosshairController : public Script {

	using base_type = CrosshairController;
	
	Camera* view;

private:

	Transform* viewTransform;
	TransformAnimator* animator;

	vec2f cursorPosition;

	void Awake() override;
	void Start() override;
	void Update() override;

	void CursorPositionHandler(Events::Event* event);
	void MouseButtonHandler(Events::Event* event);

};

#endif
