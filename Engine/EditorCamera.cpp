#include "EditorCamera.h"

#include "InputEvents.h"

#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void EditorCamera::Awake() {
	speed = 10.f;
	controlling = false;
	direction = vec3f(0.f);

	Events::EventsManager::GetInstance()->Subscribe("KEY_INPUT", &EditorCamera::KeyHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("MOUSE_BUTTON_INPUT", &EditorCamera::MouseButtonHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("CURSOR_POSITION_INPUT", &EditorCamera::CursorPositionHandler, this);
}

void EditorCamera::Update() {
	const vec3f d = direction * speed * time->dt;
	transform->translation += transform->GetLocalFront() * d.z;
	transform->translation += transform->GetLocalRight() * d.x;
	transform->translation += transform->GetLocalUp() * d.y;
}

void EditorCamera::KeyHandler(Events::Event * event) {
	if (!controlling) return;

	const auto input = static_cast<Events::KeyInput*>(event);

	if (input->action == GLFW_PRESS) {
		if (input->key == GLFW_KEY_W)
			direction.z += 1.0f;
		if (input->key == GLFW_KEY_S)
			direction.z -= 1.0f;

		if (input->key == GLFW_KEY_A)
			direction.x -= 1.0f;
		if (input->key == GLFW_KEY_D)
			direction.x += 1.0f;

		if (input->key == GLFW_KEY_Q)
			direction.y -= 1.0f;
		if (input->key == GLFW_KEY_E)
			direction.y += 1.0f;
	}

	if (input->action == GLFW_RELEASE) {
		if (input->key == GLFW_KEY_W)
			direction.z -= 1.0f;
		if (input->key == GLFW_KEY_S)
			direction.z += 1.0f;

		if (input->key == GLFW_KEY_A)
			direction.x += 1.0f;
		if (input->key == GLFW_KEY_D)
			direction.x -= 1.0f;

		if (input->key == GLFW_KEY_Q)
			direction.y += 1.0f;
		if (input->key == GLFW_KEY_E)
			direction.y -= 1.0f;
	}
}

void EditorCamera::MouseButtonHandler(Events::Event * event) {
	const auto input = static_cast<Events::MouseButtonInput*>(event);
	
	if (input->button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (input->action == GLFW_PRESS) {
			Events::EventsManager::GetInstance()->Trigger("INPUT_MODE_CHANGE", new Events::InputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED));
			controlling = true;
		} else if (input->action == GLFW_RELEASE) {
			Events::EventsManager::GetInstance()->Trigger("INPUT_MODE_CHANGE", new Events::InputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL));
			controlling = false;
			direction = vec3f(0.0f);
		}
	}
}

void EditorCamera::CursorPositionHandler(Events::Event * event) {
	if (!controlling) return;

	const auto input = static_cast<Events::CursorPositionInput*>(event);
	transform->rotation.y -= input->delta.x;
	transform->rotation.x -= input->delta.y;
	transform->UpdateAxes();
}
