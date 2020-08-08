#include "EditorCamera2D.h"

#include "InputEvents.h"

#include <Math/Math.hpp>
#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void EditorCamera2D::Awake() {
	EventsManager::Get()->Subscribe("CURSOR_POSITION_INPUT", &EditorCamera2D::CursorPositionHandler, this);
	EventsManager::Get()->Subscribe("MOUSE_BUTTON_INPUT", &EditorCamera2D::MouseButtonHandler, this);
	EventsManager::Get()->Subscribe("SCROLL_INPUT", &EditorCamera2D::ScrollHandler, this);
}

void EditorCamera2D::Start() {
	isPanning = false;
	camera = GetComponent<Camera>();
}

void EditorCamera2D::CursorPositionHandler(Events::Event * event) {
	Events::CursorPositionInput* const input = static_cast<Events::CursorPositionInput*>(event);

	const vec3f position = transform->GetLocalTranslation();
	if (isPanning) {
		vec3f delta = vec3f(input->delta.x, -input->delta.y, 0.f);
		delta *= camera->GetSize() * 0.01f;
		transform->SetLocalTranslation(position + delta);
	}
}

void EditorCamera2D::MouseButtonHandler(Events::Event * event) {
	Events::MouseButtonInput* const input = static_cast<Events::MouseButtonInput*>(event);

	if (input->button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (input->action == GLFW_PRESS) {
			StartPanning();
		} else if (input->action == GLFW_RELEASE) {
			StopPanning();
		}
	}
}

void EditorCamera2D::ScrollHandler(Events::Event * event) {
	Events::ScrollInput* const input = static_cast<Events::ScrollInput*>(event);
	camera->SetSize(Math::Clamp(camera->GetSize() - input->data.y, minZoom, maxZoom));
}

void EditorCamera2D::StartPanning() {
	isPanning = true;
	EventsManager::Get()->Trigger("INPUT_MODE_CHANGE", new Events::InputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED));
}

void EditorCamera2D::StopPanning() {
	isPanning = false;
	EventsManager::Get()->Trigger("INPUT_MODE_CHANGE", new Events::InputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL));
}
