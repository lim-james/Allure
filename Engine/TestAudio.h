#ifndef TEST_AUDIO_H
#define TEST_AUDIO_H

#include "Script.h"
#include "AudioSource.h"

#include <Events/Event.h>	

struct TestAudio : Script {

private:

	AudioSource* source;

	void Awake() override;

	void KeyHandler(Events::Event* event);
	

};

#endif
