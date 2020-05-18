#include "PlayerCombat.h"

#include "InputEvents.h"
#include "Text.h"

#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

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

void PlayerCombat::Update() {
	if (isTriggering) weapon->Hold(time->dt);
}

void PlayerCombat::MouseButtonHandler(Events::Event * event) {
	const auto input = static_cast<Events::MouseButtonInput*>(event);

	if (weapon == nullptr || input->action == GLFW_REPEAT) return;
	
	if (input->button == GLFW_MOUSE_BUTTON_LEFT) {
		if (input->action == GLFW_PRESS) {
			isTriggering = true;
			weapon->Trigger();
		} else {
			isTriggering = false;
			weapon->Release();
		}
	}
}
