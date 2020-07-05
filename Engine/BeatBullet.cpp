#include "BeatBullet.h"

#include "SpriteRender.h"
#include "Physics.h"
#include "SphereCollider.h"
#include "BulletHandler.h"

#include "LoadTexture.h"
#include "Layers.h"

BeatBullet::BeatBullet() {
	texture = Load::TGA("Files/Textures/circle.tga");
}

Transform * BeatBullet::Create() {
	const unsigned entity = entities->Create();
	entities->SetLayer(entity, BULLET);

	Transform* const transform = entities->GetComponent<Transform>(entity);
	transform->SetScale(0.5f);

	SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
	render->SetActive(true);
	render->SetSprite(texture);
	render->tint = vec4f(1.f, 0.f, 0.f, 1.f);

	Physics* const physics = entities->AddComponent<Physics>(entity);
	physics->SetActive(true);
	physics->useGravity = false;

	SphereCollider* const collider = entities->AddComponent<SphereCollider>(entity);
	collider->SetActive(true);
	//collider->ignoreMask = WEAPON | BULLET | BONUS_BULLET | ENEMY_BULLET | EFFECT_AREA;
	collider->ignoreMask = BULLET;

	BulletHandler* const bullet = entities->AddComponent<BulletHandler>(entity);
	bullet->SetActive(true);
	collider->handlers[COLLISION_ENTER].Bind(&BulletHandler::OnHit, bullet);

	return transform;
}
