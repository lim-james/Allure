#include "ButtonSystem.h"

#include "Transform.h"

#include "InputEvents.h"

#include <Events/EventsManager.h>
#include <Helpers/VectorHelpers.h>
#include <GLFW/glfw3.h>

ButtonSystem::~ButtonSystem() {
	buttons.clear();
	cameras.clear();
}

void ButtonSystem::Initialize() {
	Events::EventsManager::GetInstance()->Subscribe("CURSOR_POSITION_INPUT", &ButtonSystem::CursorPositionHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("MOUSE_BUTTON_INPUT", &ButtonSystem::MouseButtonHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("CAMERA_ACTIVE", &ButtonSystem::CameraActiveHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("BUTTON_ACTIVE", &ButtonSystem::ButtonActiveHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("WINDOW_RESIZE", &ButtonSystem::ResizeHandler, this);
}

void ButtonSystem::Update(float const& dt) {
	for (auto& cam : cameras) {
		// world space position
		const vec2f wsp = cam->ScreenToWorldSpace(mousePosition);

		for (auto& button : buttons) {
			if (!button->isEnabled) continue;

			auto& prev = states[button];

			auto transform = entities->GetComponent<Transform>(button->entity);
			const vec2f position = transform->GetWorldTranslation();
			const vec2f size = transform->scale * 0.5f;

			const vec2f offset = Math::Abs(position - wsp);

			if (offset.x <= size.x && offset.y <= size.y) {
				if (!prev) {
					PerformAction(MOUSE_OVER, button);
					prev = true;
				}

				if (mouseActions[GLFW_MOUSE_BUTTON_LEFT] != GLFW_RELEASE) {
					if (prevMouseActions[GLFW_MOUSE_BUTTON_LEFT] == GLFW_RELEASE) {
						PerformAction(MOUSE_DOWN, button);
					}
				} else {
					if (prevMouseActions[GLFW_MOUSE_BUTTON_LEFT] != GLFW_RELEASE) {
						PerformAction(MOUSE_UP, button);
						PerformAction(MOUSE_CLICK, button);
					}
				}
			} else {
				if (prev) {
					PerformAction(MOUSE_OUT, button);
					prev = false;
				}
			}
		}

		break;
	}

	for (auto& action : mouseActions) {
		prevMouseActions[action.first] = action.second;
	}
}

void ButtonSystem::CameraActiveHandler(Events::Event * event) {
	auto c = static_cast<Events::AnyType<Camera*>*>(event)->data;

	if (c->IsActive()) {
		Helpers::Insert(cameras, c);
	} else {
		Helpers::Remove(cameras, c);
	}
}

void ButtonSystem::ButtonActiveHandler(Events::Event * event) {
	auto& c = static_cast<Events::AnyType<Button*>*>(event)->data;

	if (c->IsActive()) {
		Helpers::Insert(buttons, c);
	} else {
		Helpers::Remove(buttons, c);
	}
}

void ButtonSystem::ResizeHandler(Events::Event* event) {
	windowSize = static_cast<Events::AnyType<vec2i>*>(event)->data;
}

void ButtonSystem::CursorPositionHandler(Events::Event* event) {
	Events::CursorPositionInput* input = static_cast<Events::CursorPositionInput*>(event);
	mousePosition = input->position;
	mousePosition.y = windowSize.h - mousePosition.y;
}

void ButtonSystem::MouseButtonHandler(Events::Event* event) {
	Events::MouseButtonInput* input = static_cast<Events::MouseButtonInput*>(event);
	mouseActions[input->button] = input->action;
}

void ButtonSystem::PerformAction(unsigned const& index, Button * const self) {
	self->handlers[index](self->entity);
}
