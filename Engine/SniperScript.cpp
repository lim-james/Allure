#include "SniperScript.h"

#include "SpriteRender.h"
#include "Physics.h"
#include "SphereCollider.h"
#include "SelfDestruct.h"
#include "Layers.h"

#include <Events/EventsManager.h>
#include <Math/Math.hpp>

void SniperScript::Trigger() {
	const vec2f direction = Math::Normalized(target->GetWorldTranslation() - owner->GetWorldTranslation()).xy;
	bool onBeat = false;

	if (isPlayer) {
		EventsManager::Get()->Trigger("HIT_BEAT", new Events::AnyType<bool*>(&onBeat));
		EventsManager::Get()->Trigger("SCREEN_SHAKE", new Events::AnyType<vec2f>(direction));
	}

	CreateBullet(onBeat, direction);
}

void SniperScript::Hold(float const & dt) {}

void SniperScript::Release() {}

vec3f SniperScript::HoldOffset() const {
	return vec3f(2.f, -0.5f, 0.5f);
}

std::string SniperScript::Name() const {
	return "SNIPER";
}

void SniperScript::CreateBullet(bool const& onBeat, vec2f const& direction) const {
	Transform* const bullet = bulletPrefab->Create();
	bullet->SetLocalTranslation(transform->GetWorldTranslation() + vec3f(direction));
	bullet->SetLocalRotation(vec3f(0.f, 0.f, atan2f(direction.y, direction.x) * Math::toDeg));

	Physics* const physics = entities->GetComponent<Physics>(bullet->entity);
	physics->AddForce(direction * 5000.f);

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
