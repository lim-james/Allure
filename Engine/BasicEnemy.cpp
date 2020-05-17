#include "BasicEnemy.h"

#include "SpriteRender.h"
#include "Physics.h"
#include "SphereCollider.h"
#include "StateContainer.h"
#include "EnemyLife.h"
#include "EnemyTarget.h"

#include "Layers.h"

Transform * BasicEnemy::Create() {
	const unsigned entity = entities->Create();
	entities->SetLayer(entity, ENEMY);

	Transform* const transform = entities->GetComponent<Transform>(entity);
	transform->scale = 3.f;
	
	SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
	render->SetActive(true);
	render->tint = vec4f(1.f, 0.f, 0.f, 1.f);
	
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

	EnemyTarget* const target = entities->AddComponent<EnemyTarget>(entity);
	target->SetActive(true);

	return transform;
}
