#include "Pistol.h"

#include "SpriteRender.h"
#include "SphereCollider.h"
#include "PistolScript.h"

#include "LoadTexture.h"
#include "Layers.h"

Pistol::Pistol() {
	spriteSheet = Load::Texture2D("Files/Sprites/US.png");
}

Transform * Pistol::Create() {
	const unsigned entity = entities->Create();
	entities->SetLayer(entity, WEAPON);

	Transform* const transform = entities->GetComponent<Transform>(entity);
	transform->SetScale(vec3f(6.f, 3.f, 1.f));

	SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
	render->SetActive(true);
	render->SetSprite(spriteSheet);
	render->SetTilemapSize(3, 6);
	render->SetCellRect(0, 5, 1, 1);

	SphereCollider* const collider = entities->AddComponent<SphereCollider>(entity);
	collider->SetActive(true);
	collider->ignoreMask = BULLET | BONUS_BULLET | WEAPON | ENEMY;

	entities->RemoveComponents<WeaponBase>(entity);

	PistolScript* const gun = entities->AddComponent<PistolScript>(entity);
	gun->SetActive(true);
	gun->bulletPrefab = bulletPrefab;

	return transform;
}
