#include "ShotgunScript.h"

#include "SpriteRender.h"
#include "Physics.h"
#include "SphereCollider.h"
#include "SelfDestruct.h"
#include "Layers.h"

#include <Events/EventsManager.h>
#include <Math/Math.hpp>

void ShotgunScript::Trigger() {
	const vec2f facing = Math::Normalized(target->GetWorldTranslation() - owner->GetWorldTranslation()).xy;
	bool onBeat = false;

	if (isPlayer) {
		EventsManager::Get()->Trigger("HIT_BEAT", new Events::AnyType<bool*>(&onBeat));
		EventsManager::Get()->Trigger("SCREEN_SHAKE", new Events::AnyType<vec2f>(facing));
	}

	CreateBurst(onBeat, facing);
}

void ShotgunScript::Hold(float const & dt) {}

void ShotgunScript::Release() {}

vec3f ShotgunScript::HoldOffset() const {
	return vec3f(1.5f, -0.5f, 0.5f);
}

std::string ShotgunScript::Name() const {
	return "SHOTGUN";
}

void ShotgunScript::CreateBurst(bool const& onBeat, vec2f const& facing) const {

	const vec2f normal = vec2f(facing.y, -facing.x) * 0.1f;

	for (float i = -2.f; i <= 2.f; ++i) {
		const vec2f direction = Math::Normalized(facing + normal * i);

		Transform* const bullet = bulletPrefab->Create();
		bullet->SetLocalTranslation(transform->GetWorldTranslation() + vec3f(direction));
		bullet->SetLocalRotation(vec3f(0.f, 0.f, atan2f(direction.y, direction.x) * Math::toDeg));

		Physics* const physics = entities->GetComponent<Physics>(bullet->entity);
		physics->AddForce(direction * GetForce());

		SphereCollider* const collider = entities->GetComponent<SphereCollider>(bullet->entity);
		collider->ignoreMask += bulletMask;

		entities->SetLayer(bullet->entity, onBeat ? BONUS_BULLET : bulletLayer);
	}

	const unsigned audioSource = audioPrefab->Create()->entity;
	AudioSource* const audio = entities->GetComponent<AudioSource>(audioSource);
	audio->audioClip = onBeat ? "Files/Media/base.wav" : "Files/Media/hit.wav";
	audio->Play();
}
