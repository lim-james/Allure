#include "PlayerInput.h"

#include "InputEvents.h"

#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void PlayerInput::Awake() {
	physics = GetComponent<Physics>();

	EventsManager::Get()->Subscribe("KEY_INPUT", &PlayerInput::KeyHandler, this);
}

void PlayerInput::Update() {
	physics->AddForce(direction * speed);
	//transform->translation.xy +=  * speed * time->dt;
}

void PlayerInput::KeyHandler(Events::Event* event) {
	auto input = static_cast<Events::KeyInput*>(event);

	if (input->action == GLFW_REPEAT) return;

	const float delta = 1.f - static_cast<float>(input->action == GLFW_RELEASE) * 2;

	if (input->key == GLFW_KEY_W) {
		direction.y += delta;
	} else if (input->key == GLFW_KEY_S) {
		direction.y -= delta;
	} else if (input->key == GLFW_KEY_A) {
		direction.x -= delta;
	} else if (input->key == GLFW_KEY_D) {
		direction.x += delta;
	}
}
