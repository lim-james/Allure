#include "BatPreview.h"

#include "SpriteRender.h"
#include "SpriteAnimation.h"
#include "Button.h"

#include "LoadTexture.h"
#include "LoadSAD.h"
#include "Layers.h"

BatPreview::BatPreview() {
	spriteSheet = Load::Texture2D("Files/Sprites/Bat_Sprite_Sheet.png");
	spriteData = Load::SAD("Files/Sprites/bats.sad");
}

Transform * BatPreview::Create() {
	const unsigned entity = entities->Create();
	entities->SetLayer(entity, UI);

	Transform* const transform = entities->GetComponent<Transform>(entity);
	transform->SetScale(3.f);

	SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
	render->SetActive(true);
	render->SetSprite(spriteSheet);

	SpriteAnimation* const spriteAnimation = entities->AddComponent<SpriteAnimation>(entity);
	spriteAnimation->SetActive(true);
	spriteAnimation->animations = spriteData;
	spriteAnimation->currentAnimation = "IDLE";
	spriteAnimation->animations["DEAD"].loop = false;

	Button* const button = entities->AddComponent<Button>(entity);
	button->SetActive(true);

	return transform;
}
