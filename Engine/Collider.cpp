#include "Collider.h"

Collider::Collider() 
	: isTrigger(false) 
	, scale(1.f)
	, offset(0.f) 
	, ignoreMask(0) {
	for (auto& h : handlers)
		h.UnbindAll();
}

void Collider::Initialize() {
	isTrigger = false;
	scale = 1.f;
	offset = 0.f;
	ignoreMask = 0;
	for (auto& h : handlers)
		h.UnbindAll();
}
