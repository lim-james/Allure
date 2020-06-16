#include "Shotgun.h"

#include "SpriteRender.h"
#include "ShotgunScript.h"

#include "LoadTexture.h"
#include "Layers.h"

Shotgun::Shotgun() {
	spriteSheet = Load::Texture2D("Files/Sprites/US.png");
}

Transform * Shotgun::Create() {
	const unsigned entity = entities->Create();
	entities->SetLayer(entity, WEAPON);

	Transform* const transform = entities->GetComponent<Transform>(entity);
	transform->scale.xy = vec2f(6.f, 3.f);

	SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
	render->SetActive(true);
	render->SetSprite(spriteSheet);
	render->SetTilemapSize(3, 6);
	render->SetCellRect(2, 4, 1, 1);

	ShotgunScript* const gun = entities->AddComponent<ShotgunScript>(entity);
	gun->SetActive(true);
	gun->standardPrefab = bulletPrefab;
	gun->explosivePrefab = bulletPrefab;

	return transform;
}