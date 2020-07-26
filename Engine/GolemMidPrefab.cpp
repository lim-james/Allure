#include "GolemMidPrefab.h"

#include "SpriteRender.h"
#include "SpriteAnimation.h"
#include "SphereCollider.h"
// utilities
#include "LoadTexture.h"
#include "LoadSAD.h"

GolemMidPrefab::GolemMidPrefab() {
	spriteSheet = Load::Texture2D("Files/SpritePack/Golem/Middle Golem/golem-mid-sheets-2017-02-18.png");
	spriteData = Load::SAD("Files/SpritePack/Golem/Middle Golem/golem-mid.sad");
}

Transform * GolemMidPrefab::Create() {
	const unsigned entity = entities->Create();

	Transform* const transform = entities->GetComponent<Transform>(entity);

	SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
	render->SetActive(true);
	render->SetSprite(spriteSheet);

	SpriteAnimation* const animation = entities->AddComponent<SpriteAnimation>(entity);
	animation->SetActive(true);
	animation->animations = spriteData;
	animation->currentAnimation = "IDLE";

	SphereCollider* const collider = entities->AddComponent<SphereCollider>(entity);
	collider->SetActive(true);

	return transform;
}
