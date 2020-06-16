#include "BulletHandler.h"

#include "Layers.h"

void BulletHandler::OnHit(unsigned target) {
	if (entities->GetLayer(target) == ENEMY)
		entities->Destroy(entity);
}
