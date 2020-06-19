#include "ShotgunScript.h"

#include "SpriteRender.h"
#include "Physics.h"
#include "SphereCollider.h"
#include "SelfDestruct.h"
#include "Layers.h"

#include <Events/EventsManager.h>
#include <Math/Math.hpp>

void ShotgunScript::Trigger() {
	bool onBeat = false;
	EventsManager::Get()->Trigger("HIT_BEAT", new Events::AnyType<bool*>(&onBeat));

	CreateBurst(onBeat);
}

void ShotgunScript::Hold(float const & dt) {}

void ShotgunScript::Release() {}

vec3f ShotgunScript::HoldOffset() const {
	return vec3f(1.5f, -0.5f, 0.5f);
}

void ShotgunScript::CreateBurst(bool const& onBeat) const {
	const vec2f facing = Math::Normalized(target->translation - owner->translation).xy;
	EventsManager::Get()->Trigger("SCREEN_SHAKE", new Events::AnyType<vec2f>(facing));

	const vec2f normal = vec2f(facing.y, -facing.x) * 0.1f;

	for (float i = -2.f; i <= 2.f; ++i) {
		const vec2f direction = Math::Normalized(facing + normal * i);

		Transform* const transform = bulletPrefab->Create();
		transform->translation = owner->translation;
		transform->rotation.z = atan2f(direction.y, direction.x) * Math::toDeg;

		Physics* const physics = entities->GetComponent<Physics>(transform->entity);
		physics->AddForce(direction * 5000.f);

		entities->SetLayer(transform->entity, onBeat ? BONUS_BULLET : BULLET);
	}

	audio->audioClip = onBeat ? "Files/Media/base.wav" : "Files/Media/hit.wav";
	audio->Play();
}
