#include "BasicPreview.h"

#include "SpriteRender.h"
#include "SpriteAnimation.h"
#include "EnemyCombat.h"

#include "LoadTexture.h"
#include "LoadSAD.h"
#include "Layers.h"

BasicPreview::BasicPreview() {
	spriteSheet = Load::Texture2D("Files/Sprites/Imp Sprite Sheet.png");
	spriteData = Load::SAD("Files/Sprites/imp.sad");
}

Transform * BasicPreview::Create() {

	Transform* holder = nullptr;
	{
		const unsigned entity = entities->Create();
		holder = entities->GetComponent<Transform>(entity);
		holder->SetLocalTranslation(vec3f(0.f, -0.35f, 0.f));
	}

	const unsigned entity = entities->Create();
	entities->SetLayer(entity, UI);

	Transform* const transform = entities->GetComponent<Transform>(entity);
	transform->SetScale(5.f);
	holder->SetParent(transform);

	SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
	render->SetActive(true);
	render->SetSprite(spriteSheet);

	SpriteAnimation* const spriteAnimation = entities->AddComponent<SpriteAnimation>(entity);
	spriteAnimation->SetActive(true);
	spriteAnimation->animations = spriteData;
	spriteAnimation->currentAnimation = "IDLE";
	spriteAnimation->animations["DEAD"].loop = false;

	EnemyCombat* const combat = entities->AddComponent<EnemyCombat>(entity);
	combat->weaponHolder = holder;

	return transform;
}
