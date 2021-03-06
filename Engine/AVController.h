#ifndef AV_CONTROLLER_H
#define AV_CONTROLLER_H

#include "Script.h"

#include "AudioSource.h"
#include "Text.h"
#include "Layout.h"
#include "SpriteRender.h"

#include <Audio/AudioFile.h>
#include <Handler/Handler.h>

struct AVController : Script {

	using base_type = AVController;

	AudioSource* source;

	// progress

	Text* timeLabel;
	Text* durationLabel;
	Text* beatLabel;
	
	Layout* progressIndicator;

	// controls

	Text* bpmLabel;
	Text* titleLabel;
	SpriteRender* playButton;

	void OnClickBPM();
	void ToggleAudio();
	void NextBeat();
	void PreviousBeat();

	Handler<void, int> indexChangeHandler;

	bool IsInvalid() const;

	void SetTrack(std::string const& path);

	bool IsEditingBPM() const;
	unsigned GetBPM() const;

private:

	bool editingBPM;
	unsigned bpm;
	float bps;

	int beatIndex;

	float duration;
	float bt;

	void Awake() override;
	void Start() override;
	void Update() override;

	void KeyHandler(Events::Event* event);
	void TextHandler(Events::Event* event);

	void ScrubTrack();

	std::string GetTimeString(float const& t) const;
	void UpdateProgress();

	void UpdateBPM();

};

#endif
