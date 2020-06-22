#include "Sniper.h"

#include "SpriteRender.h"
#include "SphereCollider.h"
#include "SniperScript.h"

#include "LoadTexture.h"
#include "Layers.h"

Sniper::Sniper() {
	spriteSheet = Load::Texture2D("Files/Sprites/UK.png");
}

Transform * Sniper::Create() {
	const unsigned entity = entities->Create();
	entities->SetLayer(entity, WEAPON);

	Transform* const transform = entities->GetComponent<Transform>(entity);
	transform->SetScale(vec3f(6.f, 3.f, 1.f));

	SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
	render->SetActive(true);
	render->SetSprite(spriteSheet);
	render->SetTilemapSize(4, 2);
	render->SetCellRect(2, 1, 1, 1);

	SphereCollider* const collider = entities->AddComponent<SphereCollider>(entity);
	collider->SetActive(true);
	collider->ignoreMask = BULLET | BONUS_BULLET | WEAPON | ENEMY;

	SniperScript* const gun = entities->AddComponent<SniperScript>(entity);
	gun->SetActive(true);
	gun->bulletPrefab = bulletPrefab;

	return transform;
}
