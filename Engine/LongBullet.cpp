#include "LongBullet.h"

#include "SpriteRender.h"
#include "Physics.h"
#include "SphereCollider.h"
#include "SelfDestruct.h"
#include "BulletHandler.h"

#include "LoadTexture.h"
#include "Layers.h"

LongBullet::LongBullet() {
	spriteSheet = Load::Texture2D("Files/Sprites/NTlikeTDSSprites.png");
}

Transform* LongBullet::Create() {
	const unsigned entity = entities->Create();
	entities->SetLayer(entity, BULLET);

	Transform* const transform = entities->GetComponent<Transform>(entity);
	transform->SetScale(vec3f(4.5f, 2.25f, 1.f));

	SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
	render->SetActive(true);
	render->SetSprite(spriteSheet);
	render->SetTilemapSize(5, 17);
	render->SetCellRect(0, 1, 1, 1);
	//render->tint = vec4f(0.f, 1.f, 1.f, 1.f);

	Physics* const physics = entities->AddComponent<Physics>(entity);
	physics->SetActive(true);
	physics->useGravity = false;

	SphereCollider* const collider = entities->AddComponent<SphereCollider>(entity);
	collider->SetActive(true);
	collider->ignoreMask = WEAPON | BULLET | BONUS_BULLET | ENEMY_BULLET | EFFECT_AREA;

	BulletHandler* const bullet = entities->AddComponent<BulletHandler>(entity);
	bullet->SetActive(true);
	collider->handlers[COLLISION_ENTER].Bind(&BulletHandler::OnHit, bullet);

	SelfDestruct* const destruct = entities->AddComponent<SelfDestruct>(entity);
	destruct->SetActive(true);
	destruct->lifetime = 5.f;

	return transform;
}
