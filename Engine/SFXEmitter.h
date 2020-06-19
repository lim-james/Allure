#ifndef SFX_EMITTER_H
#define SFX_EMITTER_H

#include "Prefab.h"

struct SFXEmitter : Prefab {
	Transform* Create() override;
};
	
#endif
