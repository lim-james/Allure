#include "PlayerCombat.h"

#include "InputEvents.h"
#include "Text.h"

#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void PlayerCombat::SetTempo(unsigned const & tempo) {
	delay = 60.f / static_cast<float>(tempo);
}

void PlayerCombat::SetCrosshair(Transform * const transform) {
	crosshair = transform;
	if (weapon) weapon->SetCrosshair(crosshair);
}

void PlayerCombat::SetWeapon(WeaponBase * const ptr) {
	weapon = ptr;
	if (weapon) {
		weapon->SetPlayer(transform);
		weapon->SetAudioSource(GetComponent<AudioSource>());
		weapon->SetCrosshair(crosshair);
	}
}

void PlayerCombat::Awake() {
	crosshair = nullptr;
	weapon = nullptr;
	isTriggering = false;

	EventsManager::Get()->Subscribe("MOUSE_BUTTON_INPUT", &PlayerCombat::MouseButtonHandler, this);
}

void PlayerCombat::Start() {
	bt = delay;
	isHit = false;
}

void PlayerCombat::Update() {
	if (isTriggering) weapon->Hold(time->dt);

	bt -= time->dt;

	material->interval = delay;
	material->bt = bt;
	material->threshold = threshold;
	material->playerPosition = transform->GetWorldTranslation();

	if (bt < 0.f) {
		bt = delay;
		if (!isHit) {
			Transform* const indicator = indicatorPrefab->Create();
			indicator->translation = transform->GetWorldTranslation() + vec3f(0.f, 3.f, 0.f);
		
			Text* const text = entities->GetComponent<Text>(indicator->entity);
			text->text = "MISSED";
			EventsManager::Get()->Trigger("RESET_MULTIPLIER");
		}
		isHit = false;
	}
}

void PlayerCombat::MouseButtonHandler(Events::Event * event) {
	const auto input = static_cast<Events::MouseButtonInput*>(event);

	if (weapon == nullptr || input->action == GLFW_REPEAT) return;
	
	if (input->button == GLFW_MOUSE_BUTTON_LEFT) {
		if (input->action == GLFW_PRESS) {
			isTriggering = true;
			weapon->Trigger(Hit());
		} else {
			isTriggering = false;
			weapon->Release(Hit());
		}
	}
}

bool PlayerCombat::Hit() {
	if (!isHit && threshold >= bt) {
		material->et = 0.0f;
		isHit = true;
		return true;
	} else {
		//if (isHit) {
		//	Transform* const indicator = indicatorPrefab->Create();
		//	indicator->translation = transform->GetWorldTranslation() + vec3f(0.f, 3.f, 0.f);

		//	Text* const text = entities->GetComponent<Text>(indicator->entity);
		//	text->text = "TOO EARLY";
		//}
		return false;
	}
}
