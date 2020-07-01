#ifndef BEAT_CONTROLLER_H
#define BEAT_CONTROLLER_H

#include "Script.h"
#include "AudioSource.h"
#include "Text.h"
#include "BackgroundMaterial.h"
#include "Prefab.h"

#include <Audio/AudioFile.h>
#include <Events/Event.h>	

struct BeatController : Script {

	using base_type = BeatController;

	// audio attributes

	AudioSource* source;

	unsigned startFrequency;
	unsigned endFrequency;
	unsigned frequencyBands;

	float audioDuration;

	float maxHeight;
	float* meterHeight;
	
	// references

	Text* promptLabel;
	Material::Background* background;
	Prefab* indicatorPrefab;

	// tempo attributes

	float goodThreshold;
	float greatThreshold;
	float perfectThreshold;

	unsigned perfect, great, good, missed;

	void SetTempo(unsigned const& tempo);

private: 

	// audio attributes

	AudioFile<int16_t>* file;
	float et;
	float duration;

	// tempo attributes
	
	float delay;
	float bt;

	// player position

	bool isHit; // check if this beat has been hit
	bool endCycle;

	void Awake() override;
	void Start() override;
	void Update() override;
	void FixedUpdate() override;
	void Stop() override;
	
	void HitHandler(Events::Event* event);

	void Alert(std::string const& message, vec3f const& color);

};

#endif
