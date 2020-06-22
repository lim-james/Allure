#include "Automatic.h"

#include "SpriteRender.h"
#include "SphereCollider.h"
#include "AutomaticScript.h"

#include "LoadTexture.h"
#include "Layers.h"

Automatic::Automatic() {
	spriteSheet = Load::Texture2D("Files/Sprites/USSR.png");
}

Transform * Automatic::Create() {
	const unsigned entity = entities->Create();
	entities->SetLayer(entity, WEAPON);

	Transform* const transform = entities->GetComponent<Transform>(entity);
	transform->SetScale(vec2f(6.f, 3.f));

	SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
	render->SetActive(true);
	render->SetSprite(spriteSheet);
	render->SetTilemapSize(4, 3);
	render->SetCellRect(2, 2, 1, 1);

	SphereCollider* const collider = entities->AddComponent<SphereCollider>(entity);
	collider->SetActive(true);
	collider->ignoreMask = BULLET | BONUS_BULLET | WEAPON | ENEMY;

	AutomaticScript* const gun = entities->AddComponent<AutomaticScript>(entity);
	gun->SetActive(true);
	gun->bulletPrefab = bulletPrefab;

	return transform;
}
