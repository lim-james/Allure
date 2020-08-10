#ifndef MENU_SCENE_H
#define MENU_SCENE_H

#include "Scene.h"

#include "SpectrumBubbleMaterial.h"
#include "CircleMaterial.h"
#include "AudioPlayer.h"
#include "TableViewCell.h"
#include "SFXEmitter.h"

class MenuScene : public Scene {
	Material::SpectrumBubble* spectrumBubble;
	Material::Circle* circle;
	AudioPlayer* audioPlayer;
	TableViewCell* tableViewCell;
	SFXEmitter* sfxEmitter;

	void Awake() override;
	void Create() override;
	void Destroy() override;
};

#endif
