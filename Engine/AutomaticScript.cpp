#include "AutomaticScript.h"

#include "SpriteRender.h"
#include "Physics.h"
#include "SphereCollider.h"
#include "SelfDestruct.h"
#include "Layers.h"

#include <Events/EventsManager.h>

AutomaticScript::AutomaticScript() 
	: firerate(0.075f) 
	, bt(0.f)
	, maxBurst(4)
	, fireCount(0) {}

void AutomaticScript::Trigger() {
	const vec2f direction = Math::Normalized(target->GetWorldTranslation() - owner->GetWorldTranslation()).xy;

	onBeat = false;

	if (isPlayer) {
		EventsManager::Get()->Trigger("HIT_BEAT", new Events::AnyType<bool*>(&onBeat));
		EventsManager::Get()->Trigger("SCREEN_SHAKE", new Events::AnyType<vec2f>(direction));
	} 

	CreateBullet(onBeat, direction);

	bt = firerate;
	fireCount = 1;
}

void AutomaticScript::Hold(float const & dt) {
	if (fireCount >= maxBurst) return;

	if (bt < 0.f) {
		const vec2f direction = Math::Normalized(target->GetWorldTranslation() - owner->GetWorldTranslation()).xy;
		CreateBullet(onBeat, direction);

		bt = firerate - bt;
		++fireCount;
	} else {
		bt -= dt;
	}
}

void AutomaticScript::Release() {}

vec3f AutomaticScript::HoldOffset() const {
	return vec3f(2.f, -0.5f, 0.5f);
}

std::string AutomaticScript::Name() const {
	return "AUTOMATIC";
}

void AutomaticScript::CreateBullet(bool const& onBeat, vec2f const& direction) const { 
	Transform* const bullet = bulletPrefab->Create();
	bullet->SetLocalTranslation(transform->GetWorldTranslation() + vec3f(direction));
	bullet->SetLocalRotation(vec3f(0.f, 0.f, atan2f(direction.y, direction.x) * Math::toDeg));

	Physics* const physics = entities->GetComponent<Physics>(bullet->entity);
	physics->AddForce(direction * GetForce());

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

