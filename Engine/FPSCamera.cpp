#include "FPSCamera.h"

#include "InputEvents.h"

#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void FPSCamera::Awake() {
	speed = 10.f;

	Events::EventsManager::GetInstance()->Subscribe("KEY_INPUT", &FPSCamera::KeyHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("CURSOR_POSITION_INPUT", &FPSCamera::CursorPositionHandler, this);
	Events::EventsManager::GetInstance()->Trigger("INPUT_MODE_CHANGE", new Events::InputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED));
}

void FPSCamera::KeyHandler(Events::Event * event) {
	auto input = static_cast<Events::KeyInput*>(event);

	if (input->action != GLFW_RELEASE) {
		const float d = speed * time->dt;

		if (input->key == GLFW_KEY_W)
			transform->translation += transform->GetLocalFront() * d;

		if (input->key == GLFW_KEY_S)
			transform->translation -= transform->GetLocalFront() * d;

		if (input->key == GLFW_KEY_A)
			transform->translation -= transform->GetLocalRight() * d;

		if (input->key == GLFW_KEY_D)
			transform->translation += transform->GetLocalRight() * d;
	}
}

void FPSCamera::CursorPositionHandler(Events::Event * event) {
	auto input = static_cast<Events::CursorPositionInput*>(event);
	transform->rotation.y -= input->delta.x;
	transform->rotation.x -= input->delta.y;
	transform->UpdateAxes();
}
