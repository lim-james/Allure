#include "GrenadeLauncherScript.h"

#include "SpriteRender.h"
#include "Physics.h"
#include "SphereCollider.h"
#include "SelfDestruct.h"
#include "Layers.h"

#include <Events/EventsManager.h>

void GrenadeLauncherScript::Trigger() {
	bool onBeat = false;
	EventsManager::Get()->Trigger("HIT_BEAT", new Events::AnyType<bool*>(&onBeat));

	CreateBullet(onBeat);
}

void GrenadeLauncherScript::Hold(float const & dt) {}

void GrenadeLauncherScript::Release() {}

vec3f GrenadeLauncherScript::HoldOffset() const {
	return vec3f(1.5f, -0.5f, 0.5f);
}

void GrenadeLauncherScript::CreateBullet(bool const& onBeat) const {
	const vec2f direction = Math::Normalized(crosshair->translation - player->translation).xy;

	Transform* const transform = (onBeat ? explosivePrefab : standardPrefab)->Create();
	transform->translation = player->translation;

	Physics* const physics = entities->GetComponent<Physics>(transform->entity);
	physics->AddForce(direction * 5000.f);

	EventsManager::Get()->Trigger("SCREEN_SHAKE", new Events::AnyType<vec2f>(direction));
	audio->audioClip = onBeat ? "Files/Media/base.wav" : "Files/Media/hit.wav";
	audio->Play();
}