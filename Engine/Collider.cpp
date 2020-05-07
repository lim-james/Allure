#include "Collider.h"

Collider::Collider() 
	: isTrigger(false) 
	, ignoreMask(0) {
	for (auto& h : handlers)
		h.UnbindAll();
}

void Collider::Initialize() {
	isTrigger = false;
	ignoreMask = 0;
	for (auto& h : handlers)
		h.UnbindAll();
}
