#include "PistolScript.h"

#include "SpriteRender.h"
#include "Physics.h"
#include "SphereCollider.h"
#include "SelfDestruct.h"
#include "Layers.h"

#include <Events/EventsManager.h>

void PistolScript::Trigger() {
	const vec2f direction = Math::Normalized(target->GetWorldTranslation() - owner->GetWorldTranslation()).xy;
	bool onBeat = false;

	if (isPlayer) {
		EventsManager::Get()->Trigger("HIT_BEAT", new Events::AnyType<bool*>(&onBeat));
		EventsManager::Get()->Trigger("SCREEN_SHAKE", new Events::AnyType<vec2f>(direction));
	}

	CreateBullet(onBeat, direction);
}

void PistolScript::Hold(float const & dt) {}

void PistolScript::Release() {}

vec3f PistolScript::HoldOffset() const {
	return vec3f(1.5f, -0.5f, 0.5f);
}

void PistolScript::CreateBullet(bool const& onBeat, vec2f const& direction) const { 

	Transform* const transform = bulletPrefab->Create();
	transform->SetLocalTranslation(owner->GetWorldTranslation() + vec3f(direction));

	Physics* const physics = entities->GetComponent<Physics>(transform->entity);
	physics->AddForce(direction * 5000.f);

	const unsigned audioSource = audioPrefab->Create()->entity;
	AudioSource* const audio = entities->GetComponent<AudioSource>(audioSource);

	if (onBeat) {
		entities->SetLayer(transform->entity, BONUS_BULLET);
		audio->audioClip = "Files/Media/base.wav";
	} else {
		entities->SetLayer(transform->entity, BULLET);
		audio->audioClip = "Files/Media/hit.wav";
	}

	audio->Play();
}
