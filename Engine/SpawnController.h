#ifndef SPAWN_CONTROLLER_H
#define SPAWN_CONTROLLER_H

#include "Script.h"

#include "AudioSource.h"
#include "Prefab.h"
#include "BeatMap.h"

struct SpawnController : Script {

	using base_type = SpawnController;

	float directionThreshold;
	float rotationSpeed;;

	AudioSource* source;

	Prefab* bulletPrefab;

	void SetMapPath(std::string const& filepath);

	void SetOffset(float const& t);

	void SetOuterRadius(float const& value);
	void SetInnerRadius(float const& value);
	void SetTravelTime(float const& t);

private:

	BeatMap map;

	float offset;

	float outerRadius;
	float innerRadius;
	float travelTime;
	float speed;

	float angle;
	
	void Start() override;
	void Update() override;

	void UpdateSpeed();

};

#endif
