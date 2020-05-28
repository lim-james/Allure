#ifndef AUDIO_SPECTRUM_SCENE_H
#define AUDIO_SPECTRUM_SCENE_H

#include "Scene.h"

#include "SoundBar.h"

class AudioSpectrumScene : public Scene {

	// prefabs
	SoundBar* soundBar;

private:

	void Awake() override;
	void Create() override;
	void Destroy() override;

};

#endif
