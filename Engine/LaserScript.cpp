#include "LaserScript.h"

#include "SpriteRender.h"
#include "Physics.h"
#include "SphereCollider.h"
#include "LineCollider.h"
#include "SelfDestruct.h"
#include "Layers.h"

#include <Events/EventsManager.h>

void LaserScript::Trigger() {
	const vec2f direction = Math::Normalized(target->GetWorldTranslation() - owner->GetWorldTranslation()).xy;
	bool onBeat = false;

	if (isPlayer) {
		EventsManager::Get()->Trigger("HIT_BEAT", new Events::AnyType<bool*>(&onBeat));
		EventsManager::Get()->Trigger("SCREEN_SHAKE", new Events::AnyType<vec2f>(direction));
	}

	SetLaserState(true);

	const unsigned audioSource = audioPrefab->Create()->entity;
	AudioSource* const audio = entities->GetComponent<AudioSource>(audioSource);

	if (onBeat) {
		entities->SetLayer(laserTransform->entity, BONUS_BULLET);
		audio->audioClip = "Files/Media/base.wav";
	} else {
		entities->SetLayer(laserTransform->entity, bulletLayer);
		audio->audioClip = "Files/Media/hit.wav";
	}

	audio->Play();
}

void LaserScript::Hold(float const & dt) {}

void LaserScript::Release() {
	SetLaserState(false);
}

vec3f LaserScript::HoldOffset() const {
	return vec3f(1.5f, -0.5f, 0.5f);
}

std::string LaserScript::Name() const {
	return "LASER";
}

void LaserScript::Start() {
	laserTransform = bulletPrefab->CreateIn(transform);
	laserTransform->SetLocalTranslation((laserTransform->GetScale() * 0.5f) / transform->GetScale() + vec3f(.25f, -.03f, 0.f));

	laserRender = entities->GetComponent<SpriteRender>(laserTransform->entity);
	laserCollider = entities->GetComponent<LineCollider>(laserTransform->entity);
	laserCollider->ignoreMask += bulletMask;
	SetLaserState(false);
}

void LaserScript::SetLaserState(bool const & state) {
	laserRender->SetActive(state);
	laserCollider->SetActive(state);
}
