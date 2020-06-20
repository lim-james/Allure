#include "CrosshairController.h"

#include "InputEvents.h"

#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void CrosshairController::Awake() {
	EventsManager::Get()->Subscribe("CURSOR_POSITION_INPUT", &CrosshairController::CursorPositionHandler, this);
	EventsManager::Get()->Trigger("INPUT_MODE_CHANGE", new Events::InputMode(GLFW_CURSOR, GLFW_CURSOR_HIDDEN));
}

void CrosshairController::Start() {
	viewTransform = entities->GetComponent<Transform>(view->entity);
}

void CrosshairController::Update() {
	transform->SetLocalTranslation(view->ScreenToWorldPosition(cursorPosition) + viewTransform->GetWorldTranslation().xy);
}

void CrosshairController::CursorPositionHandler(Events::Event * event) {
	const auto input = static_cast<Events::CursorPositionInput*>(event);
	cursorPosition = input->position;
}
