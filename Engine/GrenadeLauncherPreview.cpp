#include "GrenadeLauncherPreview.h"

#include "SpriteRender.h"
#include "GrenadeLauncherScript.h"

#include "LoadTexture.h"
#include "Layers.h"

GrenadeLauncherPreview::GrenadeLauncherPreview() {
	spriteSheet = Load::Texture2D("Files/Sprites/US.png");
}

Transform * GrenadeLauncherPreview::Create() {
	const unsigned entity = entities->Create();
	entities->SetLayer(entity, UI);

	Transform* const transform = entities->GetComponent<Transform>(entity);
	transform->SetScale(vec3f(6.f, 3.f, 1.f));

	SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
	render->SetActive(true);
	render->SetSprite(spriteSheet);
	render->SetTilemapSize(3, 6);
	render->SetCellRect(1, 2, 1, 1);

	GrenadeLauncherScript* const gun = entities->AddComponent<GrenadeLauncherScript>(entity);

	return transform;
}
