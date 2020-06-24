#include "BulletHandler.h"

#include "Layers.h"

void BulletHandler::Awake() {
	pierce = false;
}

void BulletHandler::OnHit(unsigned target) {
	if (!pierce) entities->Destroy(entity);
}
