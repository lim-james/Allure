#ifndef MENU_SCENE_H
#define MENU_SCENE_H

#include "Scene.h"

#include "SpectrumBubbleMaterial.h"
#include "AudioPlayer.h"
#include "TableViewCell.h"

class MenuScene : public Scene {
	Material::SpectrumBubble* spectrumBubble;
	AudioPlayer* audioPlayer;
	TableViewCell* tableViewCell;

	void Awake() override;
	void Create() override;
	void Destroy() override;
};

#endif
