#ifndef MENU_SCENE_H
#define MENU_SCENE_H

#include "Scene.h"

#include "SpectrumBubbleMaterial.h"
#include "AudioPlayer.h"

class MenuScene : public Scene {
	Material::SpectrumBubble* spectrumBubble;
	AudioPlayer* audioPlayer;

	void Awake() override;
	void Create() override;
	void Destroy() override;
};

#endif
