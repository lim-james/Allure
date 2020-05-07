#include "EnemyLife.h"

#include "Layers.h"

void EnemyLife::OnCollisionEnter(unsigned entity) {
	if (entities->GetLayer(entity) == BULLET) {
		//entities->Destroy(entity);
		emitter->Play();
	}
}

void EnemyLife::Awake() {
	emitter = GetComponent<ParticleEmitter>();
}
