#include "EnemyLife.h"

#include "Layers.h"

void EnemyLife::OnCollisionEnter(unsigned target) {
	if (entities->GetLayer(target) == BULLET) {
		entities->Destroy(target);
		entities->Destroy(entity);
		//emitter->Play();
	}
}
