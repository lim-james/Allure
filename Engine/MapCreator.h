#ifndef MAP_CREATOR_H
#define MAP_CREATOR_H

#include "Script.h"
#include "AudioSource.h"
#include "BeatMap.h"

#include <Events/Event.h>
#include <vector>

struct MapCreator : Script {

	using base_type = MapCreator;

	AudioSource* source;
	std::string savePath;

private:
	
	BeatMap map;

	void Awake() override;
	void Update() override;

	void KeyEvents(Events::Event* event);

};

#endif
