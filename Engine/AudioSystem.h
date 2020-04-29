#ifndef AUDIO_SYSTEM_H
#define AUDIO_SYSTEM_H

#include "System.h"
#include "AudioListener.h"
#include "AudioSource.h"

#include <irrKlang.h>

#include <Events/EventsManager.h>
#include <vector>

class AudioSystem : public System {

	struct Sound {
		irrklang::ISound* sound2d;
		irrklang::ISound* sound3d;

		Sound();
	};

	static int systemCount;
	static irrklang::ISoundEngine* engine;

	std::vector<AudioListener*> listeners;
	std::vector<AudioSource*> sources;

	std::map<AudioSource*, std::vector<Sound>> queued;
	std::map<AudioSource*, Sound> playing;

public:

	AudioSystem();
	~AudioSystem() override;

	void Initialize() override;
	void Update(float const& dt);

private:

	void ListenerActiveHandler(Events::Event* event);
	void SourceActiveHandler(Events::Event* event);

	void PlayHandler(Events::Event* event);
	void PauseHandler(Events::Event* event);
	void StopHandler(Events::Event* event);

};

#endif
