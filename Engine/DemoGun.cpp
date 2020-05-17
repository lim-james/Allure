#include "DemoGun.h"

#include "SpriteRender.h"
#include "Physics.h"
#include "SphereCollider.h"
#include "SelfDestruct.h"
#include "Layers.h"

#include <Events/EventsManager.h>

DemoGun::DemoGun() 
	: firerate(0.2f) 
	, bt(0.f) {}

void DemoGun::Trigger(bool const& onBeat) {
	CreateBullet(onBeat);
	bt = firerate;
}

void DemoGun::Hold(float const & dt) {
	if (bt < 0.f) {
		//CreateBullet();
		bt = firerate;
	} else {
		bt -= dt;
	}
}

void DemoGun::Release(bool const& onBeat) {}

void DemoGun::CreateBullet(bool const& onBeat) const { 
	const vec2f direction = Math::Normalized(crosshair->translation - player->translation).xy;

	Transform* const transform = (onBeat ? explosivePrefab : standardPrefab)->Create();
	transform->translation = player->translation;

	Physics* const physics = entities->GetComponent<Physics>(transform->entity);
	physics->AddForce(direction * 5000.f);

	EventsManager::Get()->Trigger("SCREEN_SHAKE", new Events::AnyType<vec2f>(direction));
	audio->audioClip = "Files/Media/hit.wav";
	audio->Play();
}
