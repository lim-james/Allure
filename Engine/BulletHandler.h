#ifndef BULLET_HANDLER_H
#define BULLET_HANDLER_H

#include "Script.h"

struct BulletHandler : Script {
	using base_type = BulletHandler;

	bool pierce;

	void Awake() override;
	virtual void OnHit(unsigned target);
};

#endif
