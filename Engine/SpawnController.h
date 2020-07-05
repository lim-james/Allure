#ifndef SPAWN_CONTROLLER_H
#define SPAWN_CONTROLLER_H

#include "Script.h"

#include "AudioSource.h"
#include "Prefab.h"
#include "BeatMap.h"

struct SpawnController : Script {

	using base_type = SpawnController;

	AudioSource* source;

	Prefab* bulletPrefab;

	void SetOuterRadius(float const& value);
	void SetInnerRadius(float const& value);
	void SetTravelTime(float const& t);
	void SetMapPath(std::string const& filepath);

private:

	float outerRadius;
	float innerRadius;
	float travelTime;
	float speed;

	BeatMap map;
	
	void Start() override;
	void Update() override;

	void UpdateSpeed();

};

#endif
