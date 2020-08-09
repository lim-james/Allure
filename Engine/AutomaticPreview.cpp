#include "AutomaticPreview.h"

#include "SpriteRender.h"
#include "AutomaticScript.h"

#include "LoadTexture.h"
#include "Layers.h"

AutomaticPreview::AutomaticPreview() {
	spriteSheet = Load::Texture2D("Files/Sprites/USSR.png");
}

Transform * AutomaticPreview::Create() {
	const unsigned entity = entities->Create();
	entities->SetLayer(entity, UI);

	Transform* const transform = entities->GetComponent<Transform>(entity);
	transform->SetScale(vec2f(6.f, 3.f));

	SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
	render->SetActive(true);
	render->SetSprite(spriteSheet);
	render->SetTilemapSize(4, 3);
	render->SetCellRect(2, 2, 1, 1);

	AutomaticScript* const gun = entities->AddComponent<AutomaticScript>(entity);

	return transform;
}
