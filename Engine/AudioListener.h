#ifndef AUDIO_LISTENER_H
#define AUDIO_LISTENER_H

#include "Component.h"

struct AudioListener : Component {

	void Initialize() override;
	Component* Clone() const override;
	void SetActive(bool const& state) override;

};

#endif
