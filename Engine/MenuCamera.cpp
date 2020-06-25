#include "MenuCamera.h"

#include "InputEvents.h"
#include <Math/Math.hpp>
#include <Events/EventsManager.h>

void MenuCamera::Awake() {
	EventsManager::Get()->Subscribe("CURSOR_POSITION_INPUT", &MenuCamera::CursorPositionHandler, this);
	EventsManager::Get()->Subscribe("WINDOW_RESIZE", &MenuCamera::WindowSizeHandler, this);
}

void MenuCamera::Start() {
	target = 0.f;
}

void MenuCamera::Update() {
	const vec3f position = transform->GetLocalTranslation();
	transform->SetLocalTranslation(vec3f(Math::Lerp(position.xy, target, time->dt * speed), position.z));
}

void MenuCamera::CursorPositionHandler(Events::Event * event) {
	Events::CursorPositionInput* input = static_cast<Events::CursorPositionInput*>(event);
	target = (input->position * windowUnit - 0.5f) * 2.f * range;
	target.y = -target.y;
}

void MenuCamera::WindowSizeHandler(Events::Event * event) {
	vec2f size = static_cast<Events::AnyType<vec2i>*>(event)->data;
	windowUnit = 1.f / size;
}
