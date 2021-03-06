#include "BasicEnemy.h"

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

BasicEnemy::BasicEnemy() {
	spriteSheet = Load::Texture2D("Files/Sprites/Imp Sprite Sheet.png");
	spriteData = Load::SAD("Files/Sprites/imp.sad");
}

Transform * BasicEnemy::Create() {

	Transform* holder = nullptr;
	{
		const unsigned entity = entities->Create();
		holder = entities->GetComponent<Transform>(entity);
		holder->SetLocalTranslation(vec3f(0.f, -0.35f, 0.f));
	}

	const unsigned entity = entities->Create();
	entities->SetLayer(entity, ENEMY);

	Transform* const transform = entities->GetComponent<Transform>(entity);
	transform->SetScale(10.f);
	holder->SetParent(transform);
	
	SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
	render->SetActive(true);
	render->SetSprite(spriteSheet);

	SpriteAnimation* const spriteAnimation = entities->AddComponent<SpriteAnimation>(entity);
	spriteAnimation->SetActive(true);
	spriteAnimation->animations = spriteData;
	spriteAnimation->currentAnimation = "IDLE";
	spriteAnimation->animations["DEAD"].loop = false;
	
	//ParticleEmitter* const emitter = entities->AddComponent<ParticleEmitter>(entity);
	//emitter->SetActive(true);
	//emitter->duration = 0.1f;
	//emitter->spawnInterval = 0.1f;
	//emitter->lifetime = 1.5f;
	//emitter->lifetimeRange = 0.2f;
	//emitter->loop = false;
	//emitter->speed = 15.f;
	//emitter->speedRange = 3.f;
	//emitter->drag = 10.f;
	//emitter->angleRange.z = 180.f;
	//emitter->burstAmount = 15;
	//emitter->positionRange.xy = 0.0f;
	//emitter->startSize = 0.3f;
	//emitter->startSizeRange = 0.3f;
	//emitter->endSize = 0.0f;
	//emitter->startColor = vec4f(1.f, 0.f, 0.f, 1.f);
	//emitter->endColor = vec4f(1.f, 0.f, 0.f, 1.f);

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
