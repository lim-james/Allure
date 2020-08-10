#include "Laser.h"

#include "SpriteRender.h"
#include "SphereCollider.h"
#include "LaserScript.h"

#include "LoadTexture.h"
#include "Layers.h"

Laser::Laser() {
	spriteSheet = Load::Texture2D("Files/Sprites/UK.png");
}

Transform * Laser::Create() {
	const unsigned entity = entities->Create();
	entities->SetLayer(entity, WEAPON);

	Transform* const transform = entities->GetComponent<Transform>(entity);
	transform->SetScale(vec3f(6.f, 3.f, 1.f));

	SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
	render->SetActive(true);
	render->SetSprite(spriteSheet);
	render->SetTilemapSize(4, 2);
	render->SetCellRect(0, 0, 1, 1);

	SphereCollider* const collider = entities->AddComponent<SphereCollider>(entity);
	collider->SetActive(true);
	collider->ignoreMask = BULLET | BONUS_BULLET | WEAPON | ENEMY;

	entities->RemoveComponents<WeaponBase>(entity);

	LaserScript* const gun = entities->AddComponent<LaserScript>(entity);
	gun->SetActive(true);
	gun->bulletPrefab = bulletPrefab;

	return transform;
}
