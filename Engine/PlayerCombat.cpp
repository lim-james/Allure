#include "PlayerCombat.h"

#include "InputEvents.h"
#include "Text.h"
#include "Animation.h"
#include "Layers.h"
#include "WeaponBase.h"

#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void PlayerCombat::SetCrosshair(Transform * const transform) {
	crosshair = transform;
	if (weapon) weapon->SetTarget(crosshair);
}

void PlayerCombat::SetHolder(Transform * const transform) {
	holder = transform;
}

void PlayerCombat::OnCollisionEnter(unsigned target) {
	switch (entities->GetLayer(target)) {
	case WEAPON:
	{
		hovering = target;
		pickupLabel->text = "[E] " + entities->GetComponent<WeaponBase>(target)->Name();
		Transform* const tTransform = entities->GetComponent<Transform>(target);
		Transform* const pTransform = entities->GetComponent<Transform>(pickupLabel->entity);
		pTransform->SetLocalTranslation(tTransform->GetWorldTranslation() + vec3f(0.f, 3.f, 0.f));
		break;
	}
	default:
		break;
	}
}

void PlayerCombat::OnCollisionExit(unsigned target) {
	switch (entities->GetLayer(target)) {
	case WEAPON:
		if (hovering == target) {
			hovering = 0;
			pickupLabel->text = "";
		}
		break;
	default:
		break;
	}
}

void PlayerCombat::Awake() {
	crosshair = nullptr;
	weapon = nullptr;
	isTriggering = false;

	spriteAnimation = GetComponent<SpriteAnimation>();

	EventsManager::Get()->Subscribe("KEY_INPUT", &PlayerCombat::KeyHandler, this);
	EventsManager::Get()->Subscribe("MOUSE_BUTTON_INPUT", &PlayerCombat::MouseButtonHandler, this);
}

void PlayerCombat::Start() {
	hovering = 0;
}

void PlayerCombat::Update() {
	if (weapon && isTriggering) weapon->Hold(time->dt);

	const vec2f diff = crosshair->GetWorldTranslation() - transform->GetWorldTranslation();

	const float direction = diff.x;
	transform->SetLocalRotation(vec3f(0.f, 90.f - 90.f * direction / fabs(direction), 0.f));

	weaponHolder->SetLocalRotation(vec3f(0.f, 0.f, atanf(diff.y / fabs(diff.x)) * Math::toDeg));;
}

void PlayerCombat::KeyHandler(Events::Event * event) {
	const auto input = static_cast<Events::KeyInput*>(event);

	if (input->key == GLFW_KEY_E && input->action == GLFW_PRESS) {
		if (hovering) {
			if (weapon) DropWeapon();
			PickupWeapon();
		}
	}
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

void PlayerCombat::PickupWeapon() {
	weapon = entities->GetComponent<WeaponBase>(hovering);

	Transform* const wTransform = entities->GetComponent<Transform>(weapon->entity);
	wTransform->SetParent(holder);
	wTransform->SetLocalTranslation(weapon->HoldOffset());

	weapon->audioPrefab = sfxPrefab;
	weapon->bulletMask = PLAYER;
	weapon->bulletLayer = BULLET;
	weapon->SetOwner(transform);
	weapon->SetTarget(crosshair);

	pickupLabel->text = "";
}

void PlayerCombat::DropWeapon() {
	Transform* const wTransform = entities->GetComponent<Transform>(weapon->entity);
	const vec3f position = wTransform->GetWorldTranslation();
	wTransform->SetParent(nullptr);
	wTransform->SetLocalTranslation(position);

	weapon->SetOwner(nullptr);
	weapon = nullptr;
}

