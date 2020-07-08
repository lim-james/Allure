#include "CustomCursor.h"

#include "InputEvents.h"
#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void CustomCursor::Awake() {
	EventsManager::Get()->Subscribe("CURSOR_POSITION_INPUT", &CustomCursor::CursorPositionHandler, this);
}

void CustomCursor::Start() {
	EventsManager::Get()->Trigger("INPUT_MODE_CHANGE", new Events::InputMode(GLFW_CURSOR, GLFW_CURSOR_HIDDEN));
}

void CustomCursor::CursorPositionHandler(Events::Event * event) {
	Events::CursorPositionInput* input = static_cast<Events::CursorPositionInput*>(event);
	transform->SetLocalTranslation(view->ScreenToWorldPosition(input->position));
}
