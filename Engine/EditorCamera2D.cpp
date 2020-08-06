#include "EditorCamera2D.h"

#include "InputEvents.h"

#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void EditorCamera2D::Awake() {
	EventsManager::Get()->Subscribe("CURSOR_POSITION_INPUT", &EditorCamera2D::CursorPositionHandler, this);
	EventsManager::Get()->Subscribe("MOUSE_BUTTON_INPUT", &EditorCamera2D::MouseButtonHandler, this);
	EventsManager::Get()->Subscribe("SCROLL_INPUT", &EditorCamera2D::ScrollHandler, this);
}

void EditorCamera2D::Start() {
	enabled = true;
	cursorPosition = vec3f(0.f);
	StopPanning();
}

void EditorCamera2D::Update() {
	const vec3f position = cursor->GetLocalTranslation();
	cursorPosition.z = position.z;
	cursor->SetLocalTranslation(Math::Lerp(position, cursorPosition, time->dt * 20.f));
}

void EditorCamera2D::CursorPositionHandler(Events::Event * event) {
	Events::CursorPositionInput* const input = static_cast<Events::CursorPositionInput*>(event);

	const vec3f position = transform->GetLocalTranslation();
	if (isPanning) {
		transform->SetLocalTranslation(position + vec3f(input->delta.x * 0.1f, -input->delta.y * 0.1f, 0.f));
	} else if (enabled) {
		cursorPosition = camera->ScreenToWorldPosition(input->position) + position.xy;
		cursorPosition.x = roundf(cursorPosition.x);
		cursorPosition.y = roundf(cursorPosition.y);
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
	camera->SetSize(max(camera->GetSize() - input->data.y, 5.f));
}

void EditorCamera2D::StartPanning() {
	isPanning = true;
	EventsManager::Get()->Trigger("INPUT_MODE_CHANGE", new Events::InputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED));
}

void EditorCamera2D::StopPanning() {
	isPanning = false;
	EventsManager::Get()->Trigger("INPUT_MODE_CHANGE", new Events::InputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL));
}
