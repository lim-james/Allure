#include "CastingCirclePrefab.h"

#include "SpriteRender.h"
#include "SpriteAnimation.h"
#include "SphereCollider.h"
// utilities
#include "LoadTexture.h"
#include "LoadSAD.h"

CastingCirclePrefab::CastingCirclePrefab() {
	spriteSheet = Load::Texture2D("Files/SpritePack/Mage/Magic circle/casting-circle-sheet.png");
	spriteData = Load::SAD("Files/SpritePack/Mage/Magic circle/casting-circle.sad");
}

Transform * CastingCirclePrefab::Create() {
	const unsigned entity = entities->Create();

	Transform* const transform = entities->GetComponent<Transform>(entity);

	SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
	render->SetActive(true);
	render->SetSprite(spriteSheet);

	SpriteAnimation* const animation = entities->AddComponent<SpriteAnimation>(entity);
	animation->SetActive(true);
	animation->animations = spriteData;
	animation->currentAnimation = "CAST";

	return transform;
}
