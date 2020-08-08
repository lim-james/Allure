#ifndef AV_CONTROLLER_H
#define AV_CONTROLLER_H

#include "Script.h"

#include "AudioSource.h"
#include "Text.h"
#include "Layout.h"
#include "SpriteRender.h"

struct AVController : Script {

	using base_type = AVController;

	AudioSource* source;

	// progress

	Text* timeLabel;
	Text* durationLabel;
	
	Layout* progressIndicator;

	// controls

	Text* bpmLabel;
	Text* titleLabel;
	SpriteRender* playButton;

	void OnClickBPM();
	void ToggleAudio();

	void SetTrack(std::string const& path);

private:

	float et, duration;

	void Awake() override;
	void Start() override;
	void Update() override;

	void KeyHandler(Events::Event* event);

	std::string GetTimeString(float const& t) const;
	void UpdateProgress();

};

#endif
