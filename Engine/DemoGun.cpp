#include "DemoGun.h"

#include "SpriteRender.h"
#include "Physics.h"
#include "SelfDestruct.h"

#include <Events/EventsManager.h>

DemoGun::DemoGun() 
	: firerate(0.2f) 
	, bt(0.f) {}

void DemoGun::Trigger() {
	CreateBullet();
	bt = firerate;
}

void DemoGun::Hold(float const & dt) {
	if (bt < 0.f) {
		CreateBullet();
		bt = firerate;
	} else {
		bt -= dt;
	}
}

void DemoGun::Release() {}

void DemoGun::CreateBullet() const { 
	const unsigned entity = entities->Create();
	
	const vec2f direction = Math::Normalized(crosshair->translation - player->translation).xy;

	Transform* const transform = entities->GetComponent<Transform>(entity);
	transform->translation = player->translation;
	transform->scale = 0.7f;

	SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
	render->SetActive(true);
	render->tint = vec4f(0.f, 1.f, 1.f, 1.f);

	Physics* const physics = entities->AddComponent<Physics>(entity);
	physics->SetActive(true);
	physics->AddForce(direction * 5000.f);
	physics->useGravity = false;

	SelfDestruct* const destruct = entities->AddComponent<SelfDestruct>(entity);
	destruct->SetActive(true);
	destruct->lifetime = 2.f;

	EventsManager::Get()->Trigger("SCREEN_SHAKE", new Events::AnyType<vec2f>(direction));
	audio->audioClip = "Files/Media/hit.wav";
	audio->Play();
}
