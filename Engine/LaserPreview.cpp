#include "LaserPreview.h"

#include "SpriteRender.h"
#include "LaserScript.h"

#include "LoadTexture.h"
#include "Layers.h"

LaserPreview::LaserPreview() {
	spriteSheet = Load::Texture2D("Files/Sprites/UK.png");
}

Transform * LaserPreview::Create() {
	const unsigned entity = entities->Create();
	entities->SetLayer(entity, UI);

	Transform* const transform = entities->GetComponent<Transform>(entity);
	transform->SetScale(vec3f(6.f, 3.f, 1.f));

	SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
	render->SetActive(true);
	render->SetSprite(spriteSheet);
	render->SetTilemapSize(4, 2);
	render->SetCellRect(0, 0, 1, 1);

	LaserScript* const gun = entities->AddComponent<LaserScript>(entity);

	return transform;
}
