#include "AltarCandlePrefab.h"

#include "SpriteRender.h"
#include "SpriteAnimation.h"
#include "SphereCollider.h"
// utilities
#include "LoadTexture.h"
#include "LoadSAD.h"

AltarCandlePrefab::AltarCandlePrefab() {
	spriteSheet = Load::Texture2D("Files/SpritePack/Environment/fire animations/altar-candle-anim-sheet.png");
	spriteData = Load::SAD("Files/SpritePack/Environment/fire animations/altar-candle.sad");
}

Transform * AltarCandlePrefab::Create() {
	const unsigned entity = entities->Create();

	Transform* const transform = entities->GetComponent<Transform>(entity);

	SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
	render->SetActive(true);
	render->SetSprite(spriteSheet);

	SpriteAnimation* const animation = entities->AddComponent<SpriteAnimation>(entity);
	animation->SetActive(true);
	animation->animations = spriteData;
	animation->currentAnimation = "ANIM";

	SphereCollider* const collider = entities->AddComponent<SphereCollider>(entity);
	collider->SetActive(true);

	return transform;
}
