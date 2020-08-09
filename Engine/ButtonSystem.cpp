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
	EventsManager* const em = EventsManager::Get();
	em->Subscribe("CURSOR_POSITION_INPUT", &ButtonSystem::CursorPositionHandler, this);
	em->Subscribe("MOUSE_BUTTON_INPUT", &ButtonSystem::MouseButtonHandler, this);
	em->Subscribe("CAMERA_ACTIVE", &ButtonSystem::CameraActiveHandler, this);
	em->Subscribe("BUTTON_ACTIVE", &ButtonSystem::ButtonActiveHandler, this);
	em->Subscribe("WINDOW_RESIZE", &ButtonSystem::ResizeHandler, this);
}

void ButtonSystem::Update(float const& dt) {
	std::vector<ButtonRaycast> raycast;

	int count = 0;

	for (auto& cam : cameras) {
		// world space position
		const vec2f wsp = cam->ScreenToWorldPosition(mousePosition);

		for (auto& button : buttons) {
			if (!button->isEnabled) continue;

			if (entities->GetLayer(button->entity) != cam->cullingMask) {
				continue;
			}

			bool& prev = states[button][cam->entity];

			Transform* const transform = entities->GetComponent<Transform>(button->entity);
			const vec3f worldTranslation = transform->GetWorldTranslation();
			const vec2f position = worldTranslation.xy + button->offset;
			const vec2f size = transform->GetScale().xy * button->scale * 0.5f;

			const vec2f offset = Math::Abs(position - wsp);

			if (offset.x <= size.x && offset.y <= size.y) {
				++count;
				InsertRaycast(ButtonRaycast{
					cam,
					button,
					worldTranslation.z,
					&prev
				}, raycast);
				if (!prev) {

					//PerformAction(MOUSE_OVER, button);
					//prev = true;
				}

				//if (mouseActions[GLFW_MOUSE_BUTTON_LEFT] != GLFW_RELEASE) {
				//	if (prevMouseActions[GLFW_MOUSE_BUTTON_LEFT] == GLFW_RELEASE) {
				//		PerformAction(MOUSE_DOWN, button);
				//	}
				//} else {
				//	if (prevMouseActions[GLFW_MOUSE_BUTTON_LEFT] != GLFW_RELEASE) {
				//		PerformAction(MOUSE_UP, button);
				//		PerformAction(MOUSE_CLICK, button);
				//	}
				//}
			} else {
				if (prev) {
					PerformAction(MOUSE_OUT, button);
					prev = false;
				}
			}
		}
	}

	if (count != raycast.size()) {
		Debug::Warn << "conflict\n";
	}

	auto it = raycast.begin();

	while (it != raycast.end()) {
		if (!(*it->prev)) {
			PerformAction(MOUSE_OVER, it->button);
			*it->prev = true;
		}

		if (mouseActions[GLFW_MOUSE_BUTTON_LEFT] != GLFW_RELEASE) {
			if (prevMouseActions[GLFW_MOUSE_BUTTON_LEFT] == GLFW_RELEASE) {
				PerformAction(MOUSE_DOWN, it->button);
			}
		} else {
			if (prevMouseActions[GLFW_MOUSE_BUTTON_LEFT] != GLFW_RELEASE) {
				PerformAction(MOUSE_UP, it->button);
				PerformAction(MOUSE_CLICK, it->button);
			}
		}

		// prevent pass through if raycast not allowed
		if (it->button->blockRaycast) {
			++it;
			break;
		} 

		++it;
	}

	while (it != raycast.end()) {
		if (*it->prev) {
			PerformAction(MOUSE_OUT, it->button);
			*it->prev = false;
		}

		++it;
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
	//mousePosition.y = windowSize.h - mousePosition.y;
}

void ButtonSystem::MouseButtonHandler(Events::Event* event) {
	Events::MouseButtonInput* input = static_cast<Events::MouseButtonInput*>(event);
	mouseActions[input->button] = input->action;
}

void ButtonSystem::InsertRaycast(ButtonRaycast const & info, std::vector<ButtonRaycast>& list) {
	const float camDepth = info.camera->GetDepth();

	for (auto it = list.begin(); it != list.end(); ++it) {
		const float itemCamDepth = it->camera->GetDepth();

		// check if camera layer is behind
		if (camDepth < itemCamDepth) {
			continue;
		} else if (camDepth == itemCamDepth) {
			if (info.zValue > it->zValue || // check if it is higher up
				(info.zValue == it->zValue && info.button->entity < it->button->entity)) {
				list.insert(it, info);
				return;
			} 		
		} else {
			list.insert(it, info);
			return;
		}
	}

	list.push_back(info);
}

void ButtonSystem::PerformAction(unsigned const& index, Button * const self) {
	self->handlers[index].Invoke(self->entity);
}
