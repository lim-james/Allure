#include "ShotgunPreview.h"

#include "SpriteRender.h"
#include "ShotgunScript.h"

#include "LoadTexture.h"
#include "Layers.h"

ShotgunPreview::ShotgunPreview() {
	spriteSheet = Load::Texture2D("Files/Sprites/US.png");
}

Transform * ShotgunPreview::Create() {
	const unsigned entity = entities->Create();
	entities->SetLayer(entity, UI);

	Transform* const transform = entities->GetComponent<Transform>(entity);
	transform->SetScale(vec3f(6.f, 3.f, 1.f));

	SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
	render->SetActive(true);
	render->SetSprite(spriteSheet);
	render->SetTilemapSize(3, 6);
	render->SetCellRect(2, 4, 1, 1);

	ShotgunScript* const gun = entities->AddComponent<ShotgunScript>(entity);

	return transform;
}