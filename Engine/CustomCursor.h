#ifndef CUSTOM_CURSOR_H
#define CUSTOM_CURSOR_H

#include "Script.h"
#include "Camera.h"

#include <Events/Event.h>

struct CustomCursor : Script {

	using base_type = CustomCursor;

	Camera* view;

private:

	void Awake() override;
	void Start() override;

	void CursorPositionHandler(Events::Event* event);

};

#endif
