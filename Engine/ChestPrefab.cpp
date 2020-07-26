#include "ChestPrefab.h"

#include "SpriteRender.h"
#include "SphereCollider.h"
// utilities
#include "LoadTexture.h"

ChestPrefab::ChestPrefab() {
	spriteSheet = Load::Texture2D("Files/SpritePack/Environment/chest.png");
}

Transform * ChestPrefab::Create() {
	const unsigned entity = entities->Create();

	Transform* const transform = entities->GetComponent<Transform>(entity);
	transform->SetScale(0.5f);

	SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
	render->SetActive(true);
	render->SetSprite(spriteSheet);

	SphereCollider* const collider = entities->AddComponent<SphereCollider>(entity);
	collider->SetActive(true);

	return transform;
}
