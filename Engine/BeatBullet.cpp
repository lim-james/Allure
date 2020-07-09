#include "BeatBullet.h"

#include "TransformAnimator.h"
#include "SpriteRender.h"
#include "Animator.h"
#include "StateContainer.h"
#include "Physics.h"
#include "SphereCollider.h"
#include "BeatCollisionHandler.h"

#include "LoadTexture.h"
#include "Layers.h"

BeatBullet::BeatBullet()
	: texture(Load::TGA("Files/Textures/circle.tga"))
	, material(nullptr) {}

Transform * BeatBullet::Create() {
	const unsigned entity = entities->Create();
	entities->SetLayer(entity, BULLET);

	Transform* const transform = entities->GetComponent<Transform>(entity);
	transform->SetScale(0.5f);

	TransformAnimator* const tAnimator = entities->AddComponent<TransformAnimator>(entity);
	tAnimator->SetActive(true);

	SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
	render->SetActive(true);
	render->SetMaterial(material);
	render->SetSprite(texture);
	render->tint = vec4f(1.f, 0.f, 0.f, 0.f);

	Animator* const animator = entities->AddComponent<Animator>(entity);
	animator->SetActive(true);

	StateContainer* const container = entities->AddComponent<StateContainer>(entity);
	container->SetActive(true);
	container->queuedState = "BEAT_SPAWN";

	Physics* const physics = entities->AddComponent<Physics>(entity);
	physics->useGravity = false;

	SphereCollider* const collider = entities->AddComponent<SphereCollider>(entity);
	collider->SetActive(true);
	//collider->ignoreMask = WEAPON | BULLET | BONUS_BULLET | ENEMY_BULLET | EFFECT_AREA;
	collider->ignoreMask = BULLET;

	BeatCollisionHandler* const handler = entities->AddComponent<BeatCollisionHandler>(entity);
	handler->SetActive(true);
	handler->fadeInDuration = 1.f;
	handler->fadeOutDuration = 0.25f;
	handler->material = material;

	collider->handlers[COLLISION_ENTER].Bind(&BeatCollisionHandler::OnHit, handler);

	return transform;
}
