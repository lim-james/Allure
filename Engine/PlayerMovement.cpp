#include "PlayerMovement.h"

#include "InputEvents.h"

#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void PlayerMovement::Awake() {
	emitter = GetComponent<ParticleEmitter>();
	physics = GetComponent<Physics>();

	EventsManager::Get()->Subscribe("KEY_INPUT", &PlayerMovement::KeyHandler, this);
}

void PlayerMovement::Update() {
	physics->AddForce(direction * speed * dashMagnitude);
	dashMagnitude = 1.f;
}

void PlayerMovement::KeyHandler(Events::Event* event) {
	auto input = static_cast<Events::KeyInput*>(event);

	if (input->action == GLFW_REPEAT) return;

	const float delta = 1.f - static_cast<float>(input->action == GLFW_RELEASE) * 2;

	if (input->key == GLFW_KEY_W) {
		axes.y += delta;
	} else if (input->key == GLFW_KEY_S) {
		axes.y -= delta;
	} else if (input->key == GLFW_KEY_A) {
		axes.x -= delta;
	} else if (input->key == GLFW_KEY_D) {
		axes.x += delta;
	}

	direction = Math::Normalized(axes);

	if (input->key == GLFW_KEY_SPACE && input->action == GLFW_PRESS) {
		dashMagnitude = dash;
		if (axes.x != 0.f || axes.y != 0.f) emitter->Play();
	}
}
