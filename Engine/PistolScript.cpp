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

std::string PistolScript::Name() const {
	return "PISTOL";
}

void PistolScript::CreateBullet(bool const& onBeat, vec2f const& direction) const { 
	Transform* const bullet = bulletPrefab->Create();
	bullet->SetLocalTranslation(transform->GetWorldTranslation() + vec3f(direction));

	Physics* const physics = entities->GetComponent<Physics>(bullet->entity);
	physics->AddForce(direction * 0.5f * GetForce());

	SphereCollider* const collider = entities->GetComponent<SphereCollider>(bullet->entity);
	collider->ignoreMask += bulletMask;

	const unsigned audioSource = audioPrefab->Create()->entity;
	AudioSource* const audio = entities->GetComponent<AudioSource>(audioSource);

	if (onBeat) {
		entities->SetLayer(bullet->entity, BONUS_BULLET);
		audio->audioClip = "Files/Media/base.wav";
	} else {
		entities->SetLayer(bullet->entity, bulletLayer);
		audio->audioClip = "Files/Media/hit.wav";
	}

	audio->Play();
}
