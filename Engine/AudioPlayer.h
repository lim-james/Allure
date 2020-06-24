#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include "Prefab.h"

struct AudioPlayer : Prefab {
	Transform* Create() override;
};

#endif
