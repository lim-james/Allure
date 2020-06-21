#include "BulletHandler.h"

#include "Layers.h"

void BulletHandler::OnHit(unsigned target) {
	entities->Destroy(entity);
}
