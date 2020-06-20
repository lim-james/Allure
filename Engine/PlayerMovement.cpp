#include "PlayerMovement.h"

#include "InputEvents.h"

#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void PlayerMovement::Awake() {
	spriteAnimation = GetComponent<SpriteAnimation>();
	emitter = GetComponent<ParticleEmitter>();
	physics = GetComponent<Physics>();

	EventsManager::Get()->Subscribe("KEY_INPUT", &PlayerMovement::KeyHandler, this);
}

void PlayerMovement::Update() {
	const vec3f innerBounds = bounds - transform->GetScale() * 0.5f;
	const vec3f translation = transform->GetLocalTranslation();

	transform->SetLocalTranslation(vec3f(
		Math::Clamp(translation.x, -innerBounds.x, innerBounds.x),
		Math::Clamp(translation.y, -innerBounds.y, innerBounds.y),
		Math::Clamp(translation.z, -innerBounds.z, innerBounds.z)
	));
}

void PlayerMovement::FixedUpdate() {
	physics->AddForce(direction * speed * dashMagnitude);
	dashMagnitude = 1.f;
}

void PlayerMovement::KeyHandler(Events::Event* event) {
	const auto input = static_cast<Events::KeyInput*>(event);

	if (input->action == GLFW_REPEAT) return;

	if (input->key == GLFW_KEY_I && input->action == GLFW_PRESS) {
		physics->interpolate = !physics->interpolate;
	}

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

	const float length = Math::Length(axes);
	if (length == 0.f) {
		direction = 0.f;
		spriteAnimation->queued = "IDLE";
	} else {
		direction = axes / length;
		spriteAnimation->queued = "WALK";
	}

	if (input->key == GLFW_KEY_SPACE && input->action == GLFW_PRESS) {
		dashMagnitude = dash;
		if (axes.x != 0.f || axes.y != 0.f) emitter->Play();
	}
}
