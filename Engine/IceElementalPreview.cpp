#include "IceElementalPreview.h"

#include "SpriteRender.h"
#include "SpriteAnimation.h"
#include "Button.h"

#include "LoadTexture.h"
#include "LoadSAD.h"
#include "Layers.h"

IceElementalPreview::IceElementalPreview() {
	spriteSheet = Load::Texture2D("Files/Sprites/Ice Elemental Sprite Sheet.png");
	spriteData = Load::SAD("Files/Sprites/ice_elemental.sad");
}

Transform * IceElementalPreview::Create() {
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

	Button* const button = entities->AddComponent<Button>(entity);
	button->SetActive(true);

	return transform;
}
