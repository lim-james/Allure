#include "EnemyManager.h"

void EnemyManager::Awake() {
	bt = 0.f;
	spawnDelay = 1.f;
}

void EnemyManager::Update() {
	if (bt > spawnDelay) {
		bt = 0.f;
	} else {
		bt += time->dt;
	}
}

void EnemyManager::CreateEnemy() {
	//const unsigned entity = entities->Create();
	//entities->SetLayer(entity, ENEMY);

	//Transform* const transform = entities->GetComponent<Transform>(entity);
	//transform->translation = vec3f(5.f, 0.f, 0.f);

	//SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
	//render->SetActive(true);
	//render->tint = vec4f(1.f, 0.f, 0.f, 1.f);

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

	//Physics* const physics = entities->AddComponent<Physics>(entity);
	//physics->SetActive(true);
	//physics->useGravity = false;
	//physics->drag = 10.f;

	//SphereCollider* const collider = entities->AddComponent<SphereCollider>(entity);
	//collider->SetActive(true);

	//EnemyLife* const life = entities->AddComponent<EnemyLife>(entity);
	//life->SetActive(true);
	//collider->handlers[COLLISION_ENTER].Bind(&EnemyLife::OnCollisionEnter, life);
}
