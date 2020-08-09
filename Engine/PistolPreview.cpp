#include "PistolPreview.h"

#include "SpriteRender.h"
#include "PistolScript.h"

#include "LoadTexture.h"
#include "Layers.h"

PistolPreview::PistolPreview() {
	spriteSheet = Load::Texture2D("Files/Sprites/US.png");
}

Transform * PistolPreview::Create() {
	const unsigned entity = entities->Create();
	entities->SetLayer(entity, UI);

	Transform* const transform = entities->GetComponent<Transform>(entity);
	transform->SetScale(vec3f(6.f, 3.f, 1.f));

	SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
	render->SetActive(true);
	render->SetSprite(spriteSheet);
	render->SetTilemapSize(3, 6);
	render->SetCellRect(0, 5, 1, 1);

	PistolScript* const gun = entities->AddComponent<PistolScript>(entity);

	return transform;
}
