#include "GrenadeLauncher.h"

#include "SpriteRender.h"
#include "GrenadeLauncherScript.h"

#include "LoadTexture.h"
#include "Layers.h"

GrenadeLauncher::GrenadeLauncher() {
	spriteSheet = Load::Texture2D("Files/Sprites/US.png");
}

Transform * GrenadeLauncher::Create() {
	const unsigned entity = entities->Create();
	entities->SetLayer(entity, WEAPON);

	Transform* const transform = entities->GetComponent<Transform>(entity);
	transform->scale.xy = vec2f(6.f, 3.f);

	SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
	render->SetActive(true);
	render->SetSprite(spriteSheet);
	render->SetTilemapSize(3, 6);
	render->SetCellRect(1, 2, 1, 1);

	GrenadeLauncherScript* const gun = entities->AddComponent<GrenadeLauncherScript>(entity);
	gun->SetActive(true);
	gun->standardPrefab = bulletPrefab;
	gun->explosivePrefab = bulletPrefab;

	return transform;
}
