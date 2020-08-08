#include "CrosshairController.h"

#include "InputEvents.h"

#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void CrosshairController::Awake() {
	EventsManager::Get()->Subscribe("CURSOR_POSITION_INPUT", &CrosshairController::CursorPositionHandler, this);
	EventsManager::Get()->Subscribe("MOUSE_BUTTON_INPUT", &CrosshairController::MouseButtonHandler, this);
	EventsManager::Get()->Trigger("INPUT_MODE_CHANGE", new Events::InputMode(GLFW_CURSOR, GLFW_CURSOR_HIDDEN));
}

void CrosshairController::Start() {
	viewTransform = entities->GetComponent<Transform>(view->entity);
	animator = GetComponent<TransformAnimator>();
}

void CrosshairController::Update() {
	transform->SetLocalTranslation(view->ScreenToWorldPosition(cursorPosition) + viewTransform->GetWorldTranslation().xy);
}

void CrosshairController::CursorPositionHandler(Events::Event * event) {
	const auto input = static_cast<Events::CursorPositionInput*>(event);
	cursorPosition = input->position;
}

void CrosshairController::MouseButtonHandler(Events::Event * event) {
	const auto input = static_cast<Events::MouseButtonInput*>(event);
	if (input->button == GLFW_MOUSE_BUTTON_LEFT && input->action == GLFW_PRESS) {
		animator->Clear();
		animator->Queue(AnimationBase(false, 0.1f), transform, ANIMATE_SCALE, vec3f(2.f));
		animator->Queue(AnimationBase(false, 0.1f), transform, ANIMATE_SCALE, vec3f(1.25f));
	}
}
