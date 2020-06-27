#ifndef AUDIO_CONTROLLLER_H
#define AUDIO_CONTROLLLER_H

#include "Script.h"
#include "AudioSource.h"

#include <Handler/Handler.h>

struct AudioController : Script {

	using base_type = AudioController;

	float fadeSpeed;

	void FadeIn(std::string const& path);
	void FadeOut(Handler<void, void> const& completion = nullptr);

private:

	AudioSource* source;

	// audio state
	// -1 - fade in
	//  0 - neutral
	//  1 - fade out
	int state;

	Handler<void, void> completion;

	void Awake() override;
	void Update() override;

};

#endif
