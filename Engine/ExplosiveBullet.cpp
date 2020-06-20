#include "ExplosiveBullet.h"

#include "SpriteRender.h"
#include "Physics.h"
#include "SphereCollider.h"
#include "SelfDestruct.h"
#include "BulletHandler.h"
#include "ExplosiveScript.h"

#include "LoadTexture.h"
#include "Layers.h"

ExplosiveBullet::ExplosiveBullet() {
	spriteSheet = Load::Texture2D("Files/Sprites/NTlikeTDSSprites.png");
}

Transform * ExplosiveBullet::Create() {
	const unsigned entity = entities->Create();
	entities->SetLayer(entity, BULLET);

	Transform* const transform = entities->GetComponent<Transform>(entity);
	transform->scale = 2.f;

	SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
	render->SetActive(true);
	render->SetSprite(spriteSheet);
	render->SetTilemapSize(58, 55);
	render->SetCellRect(15, 13, 9, 4);
	//render->tint = vec4f(1.f, 0.f, 1.f, 1.f);

	Physics* const physics = entities->AddComponent<Physics>(entity);
	physics->SetActive(true);
	physics->useGravity = false;

	SphereCollider* const collider = entities->AddComponent<SphereCollider>(entity);
	collider->SetActive(true);
	collider->ignoreMask = WEAPON & BULLET & BONUS_BULLET & EFFECT_AREA;

	SelfDestruct* const destruct = entities->AddComponent<SelfDestruct>(entity);
	destruct->SetActive(true);
	destruct->lifetime = 2.f;

	BulletHandler* const bullet = entities->AddComponent<BulletHandler>(entity);
	bullet->SetActive(true);
	collider->handlers[COLLISION_ENTER].Bind(&BulletHandler::OnHit, bullet);

	ExplosiveScript* const explosive = entities->AddComponent<ExplosiveScript>(entity);
	explosive->explosionPrefab = explosionPrefab;
	explosive->SetActive(true);

	return transform;
}
