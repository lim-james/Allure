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
	audio->audioClip = onBeat ? "Files/Media/base.wav" : "Files/Media/hit.wav";
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
	laserTransform = standardPrefab->CreateIn(transform);
	laserTransform->translation = (laserTransform->scale * 0.5f) / transform->scale + vec3f(.25f, -.03f, 0.f);

	laserRender = entities->GetComponent<SpriteRender>(laserTransform->entity);
	laserCollider = entities->GetComponent<LineCollider>(laserTransform->entity);
	SetLaserState(false);
}

void LaserScript::SetLaserState(bool const & state) {
	laserRender->SetActive(state);
	laserCollider->SetActive(state);
}
