#include "SelfDestruct.h"

void SelfDestruct::Start() {
	et = 0.f;
}

void SelfDestruct::Update() {
	et += time->dt;
	if (et > lifetime)
		entities->Destroy(entity);
}
