#include "Candle1Prefab.h"

#include "SpriteRender.h"
#include "SpriteAnimation.h"
#include "SphereCollider.h"
// utilities
#include "LoadTexture.h"
#include "LoadSAD.h"

Candle1Prefab::Candle1Prefab() {
	spriteSheet = Load::Texture2D("Files/SpritePack/Environment/fire animations/torches-anim-sheet.png");
	spriteData = Load::SAD("Files/SpritePack/Environment/fire animations/torches.sad");
}

Transform * Candle1Prefab::Create() {
	const unsigned entity = entities->Create();

	Transform* const transform = entities->GetComponent<Transform>(entity);
	transform->SetScale(vec3f(0.333f, 1.f, 1.f));

	SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
	render->SetActive(true);
	render->SetSprite(spriteSheet);

	SpriteAnimation* const animation = entities->AddComponent<SpriteAnimation>(entity);
	animation->SetActive(true);
	animation->animations = spriteData;
	animation->currentAnimation = "CANDLE_1";

	SphereCollider* const collider = entities->AddComponent<SphereCollider>(entity);
	collider->SetActive(true);

	return transform;
}
