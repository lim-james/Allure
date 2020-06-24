#include "CyclopsEnemy.h"

#include "SpriteRender.h"
#include "SpriteAnimation.h"
#include "ParticleEmitter.h"
#include "Physics.h"
#include "SphereCollider.h"
#include "StateContainer.h"
#include "EnemyLife.h"
#include "EnemyTarget.h"
#include "EnemyCombat.h"

#include "LoadTexture.h"
#include "LoadSAD.h"
#include "Layers.h"

CyclopsEnemy::CyclopsEnemy() {
	spriteSheet = Load::Texture2D("Files/Sprites/Cyclops Sprite Sheet.png");
	spriteData = Load::SAD("Files/Sprites/cyclops.sad");
}

Transform * CyclopsEnemy::Create() {
	Transform* holder = nullptr;
	{
		const unsigned entity = entities->Create();
		holder = entities->GetComponent<Transform>(entity);
		holder->SetLocalTranslation(vec3f(0.f, -0.35f, 0.f));
	}

	const unsigned entity = entities->Create();
	entities->SetLayer(entity, ENEMY);

	Transform* const transform = entities->GetComponent<Transform>(entity);
	transform->SetScale(20.f);
	holder->SetParent(transform);

	SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
	render->SetActive(true);
	render->SetSprite(spriteSheet);

	SpriteAnimation* const spriteAnimation = entities->AddComponent<SpriteAnimation>(entity);
	spriteAnimation->SetActive(true);
	spriteAnimation->animations = spriteData;
	spriteAnimation->currentAnimation = "IDLE";
	spriteAnimation->animations["DEAD"].loop = false;

	Physics* const physics = entities->AddComponent<Physics>(entity);
	physics->SetActive(true);
	physics->useGravity = false;
	physics->drag = 10.f;

	SphereCollider* const collider = entities->AddComponent<SphereCollider>(entity);
	collider->SetActive(true);
	collider->ignoreMask = ENEMY;

	StateContainer* const states = entities->AddComponent<StateContainer>(entity);
	states->SetActive(true);
	states->queuedState = "CHASE";

	EnemyLife* const life = entities->AddComponent<EnemyLife>(entity);
	life->SetActive(true);

	collider->handlers[COLLISION_ENTER].Bind(&EnemyLife::OnCollisionEnter, life);
	//spriteAnimation->animations["DEAD"].completed.Bind(&EnemyLife::Kill, life);

	EnemyTarget* const target = entities->AddComponent<EnemyTarget>(entity);
	target->SetActive(true);

	EnemyCombat* const combat = entities->AddComponent<EnemyCombat>(entity);
	combat->SetActive(true);
	combat->weaponHolder = holder;

	return transform;
}
