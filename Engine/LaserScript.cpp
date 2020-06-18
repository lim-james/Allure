#include "LaserScript.h"

#include "SpriteRender.h"
#include "Physics.h"
#include "LineCollider.h"
#include "SelfDestruct.h"
#include "Layers.h"

#include <Events/EventsManager.h>

void LaserScript::Trigger() {
	bool onBeat = false;
	EventsManager::Get()->Trigger("HIT_BEAT", new Events::AnyType<bool*>(&onBeat));

	SetLaserState(true);

	const vec2f direction = Math::Normalized(crosshair->translation - player->translation).xy;
	EventsManager::Get()->Trigger("SCREEN_SHAKE", new Events::AnyType<vec2f>(direction));

	if (onBeat) {
		entities->SetLayer(laserTransform->entity, BONUS_BULLET);
		audio->audioClip = "Files/Media/base.wav";
	} else {
		entities->SetLayer(laserTransform->entity, BULLET);
		audio->audioClip = "Files/Media/hit.wav";
	}

	audio->Play();
}

void LaserScript::Hold(float const & dt) {
	laserTransform->UpdateAxes();
}

void LaserScript::Release() {
	SetLaserState(false);
}

vec3f LaserScript::HoldOffset() const {
	return vec3f(1.5f, -0.5f, 0.5f);
}

void LaserScript::Start() {
	laserTransform = bulletPrefab->CreateIn(transform);
	laserTransform->translation = (laserTransform->scale * 0.5f) / transform->scale + vec3f(.25f, -.03f, 0.f);

	laserRender = entities->GetComponent<SpriteRender>(laserTransform->entity);
	laserCollider = entities->GetComponent<LineCollider>(laserTransform->entity);
	SetLaserState(false);
}

void LaserScript::SetLaserState(bool const & state) {
	laserRender->SetActive(state);
	laserCollider->SetActive(state);
}
