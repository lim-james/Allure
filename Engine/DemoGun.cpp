#include "DemoGun.h"

#include "SpriteRender.h"
#include "Physics.h"
#include "SelfDestruct.h"

DemoGun::DemoGun() 
	: firerate(0.05f) 
	, bt(0.f) {}

void DemoGun::Trigger() {
	CreateBullet();
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

	const vec3f direction = Math::Normalized(crosshair->translation - player->translation);
	
	Transform* const transform = entities->GetComponent<Transform>(entity);
	transform->translation = player->translation;
	transform->scale = 0.6f;

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
}
