#ifndef BUBBLE_MANAGER_H
#define BUBBLE_MANAGER_H

#include "Script.h"
#include "SpectrumBubbleMaterial.h"
#include "Prefab.h"
#include "AudioControlller.h"

#include <Audio/AudioFile.h>
#include <Events/Event.h>
#include <vector>

struct BubbleManager : Script {

	using base_type = BubbleManager;

	unsigned startFrequency;
	unsigned endFrequency;
	float audioDuration;

	Material::SpectrumBubble* material;
	Prefab* audioPrefab;

	BubbleManager();
	
	void AddSong(std::string const& path);

	void NextSong();
	void PreviousSong();

private:

	float t;
	float duration;

	AudioSource* source;
	AudioController* controller;
	AudioFile<int16_t>* file;

	bool switched;
	float selectionDelay;
	float bt;
	int selected;
	std::vector<std::string> songPaths;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;

	void KeyHandler(Events::Event* event);

	void SwitchingSong();
	void UpdateSong();

};

#endif
