#include "SniperPreview.h"

#include "SpriteRender.h"
#include "SniperScript.h"

#include "LoadTexture.h"
#include "Layers.h"

SniperPreview::SniperPreview() {
	spriteSheet = Load::Texture2D("Files/Sprites/UK.png");
}

Transform * SniperPreview::Create() {
	const unsigned entity = entities->Create();
	entities->SetLayer(entity, UI);

	Transform* const transform = entities->GetComponent<Transform>(entity);
	transform->SetScale(vec3f(6.f, 3.f, 1.f));

	SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
	render->SetActive(true);
	render->SetSprite(spriteSheet);
	render->SetTilemapSize(4, 2);
	render->SetCellRect(2, 1, 1, 1);

	SniperScript* const gun = entities->AddComponent<SniperScript>(entity);

	return transform;
}
