#include "Panel.h"

#include "InputEvents.h"

#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void Panel::Awake() {
	camera = nullptr;
	draggable = true;

	isDragging = false;

	Events::EventsManager::GetInstance()->Subscribe("MOUSE_BUTTON_INPUT", &Panel::MouseButtonHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("CURSOR_POSITION_INPUT", &Panel::CursorPositionHandler, this);
}

void Panel::Update() {
}

void Panel::MouseButtonHandler(Events::Event * event) {
	const auto input = static_cast<Events::MouseButtonInput*>(event);

	if (input->button == GLFW_MOUSE_BUTTON_LEFT) {
		if (input->action == GLFW_PRESS) {
			const vec2f delta = Math::Abs(prevPosition - transform->translation.xy) * 2.f;
			if (delta.x <= transform->scale.x && delta.y <= transform->scale.y) {
				isDragging = draggable;
			}

		} else if (input->action == GLFW_RELEASE) {
			isDragging = false;
		}
	}
}

void Panel::CursorPositionHandler(Events::Event * event) {
	const auto input = static_cast<Events::CursorPositionInput*>(event);
	const vec2f curr = camera->ScreenToWorldPosition(input->position);
	if (isDragging) {
		vec2f delta = curr - prevPosition;
		delta.y = -delta.y;
		transform->translation.xy += delta;
	}
	prevPosition = curr;
}
