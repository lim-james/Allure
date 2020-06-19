#include "PlayerCombat.h"

#include "InputEvents.h"
#include "Text.h"

#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void PlayerCombat::SetCrosshair(Transform * const transform) {
	crosshair = transform;
	if (weapon) weapon->SetTarget(crosshair);
}

void PlayerCombat::SetWeapon(WeaponBase * const ptr) {
	weapon = ptr;
	if (weapon) {
		weapon->SetOwner(transform);
		weapon->SetAudioSource(GetComponent<AudioSource>());
		weapon->SetTarget(crosshair);
	}
}

void PlayerCombat::Awake() {
	crosshair = nullptr;
	weapon = nullptr;
	isTriggering = false;

	spriteAnimation = GetComponent<SpriteAnimation>();

	EventsManager::Get()->Subscribe("MOUSE_BUTTON_INPUT", &PlayerCombat::MouseButtonHandler, this);
}

void PlayerCombat::Update() {
	if (weapon && isTriggering) weapon->Hold(time->dt);

	const vec2f diff = crosshair->GetWorldTranslation() - transform->GetWorldTranslation();

	const float direction = diff.x;
	transform->rotation.y = 90.f - 90.f * direction / fabs(direction);

	weaponHolder->rotation.z = atanf(diff.y / fabs(diff.x)) * Math::toDeg;
}

void PlayerCombat::MouseButtonHandler(Events::Event * event) {
	const auto input = static_cast<Events::MouseButtonInput*>(event);

	if (weapon == nullptr || input->action == GLFW_REPEAT) return;
	
	if (input->button == GLFW_MOUSE_BUTTON_LEFT) {
		if (input->action == GLFW_PRESS) {
			isTriggering = true;
			if (weapon) weapon->Trigger();
		} else {
			isTriggering = false;
			if (weapon) weapon->Release();
		}
	}
}
