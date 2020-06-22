#ifndef BEAT_CONTROLLER_H
#define BEAT_CONTROLLER_H

#include "Script.h"
#include "BackgroundMaterial.h"
#include "Prefab.h"

struct BeatController : Script {

	using base_type = BeatController;
	
	Material::Background* background;
	Prefab* indicatorPrefab;

	float threshold;

	void SetTempo(unsigned const& tempo);

private: 
	
	float delay;
	float bt;

	bool isHit; // check if this beat has been hit
	bool endCycle;
	vec3f playerPosition;

	void Awake() override;
	void Start() override;
	void Update() override;
	
	void HitHandler(Events::Event* event);

};

#endif
