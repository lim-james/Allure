#ifndef SELF_DESTRUCT_H
#define SELF_DESTRUCT_H

#include "Script.h"

struct SelfDestruct : Script {

	using base_type = SelfDestruct;

	float lifetime;

private:

	float et;

	void Start() override;
	void Update() override;

};

#endif
