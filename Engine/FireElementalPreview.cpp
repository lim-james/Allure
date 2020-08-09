#include "FireElementalPreview.h"

#include "SpriteRender.h"
#include "SpriteAnimation.h"

#include "LoadTexture.h"
#include "LoadSAD.h"
#include "Layers.h"

FireElementalPreview::FireElementalPreview() {
	spriteSheet = Load::Texture2D("Files/Sprites/Fire Elemental Sprite Sheet.png");
	spriteData = Load::SAD("Files/Sprites/fire_elemental.sad");
}

Transform * FireElementalPreview::Create() {
	const unsigned entity = entities->Create();
	entities->SetLayer(entity, UI);

	Transform* const transform = entities->GetComponent<Transform>(entity);
	transform->SetScale(4.f);

	SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
	render->SetActive(true);
	render->SetSprite(spriteSheet);

	SpriteAnimation* const spriteAnimation = entities->AddComponent<SpriteAnimation>(entity);
	spriteAnimation->SetActive(true);
	spriteAnimation->animations = spriteData;
	spriteAnimation->currentAnimation = "IDLE";
	spriteAnimation->animations["DEAD"].loop = false;

	return transform;
}
