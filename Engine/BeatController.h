#ifndef BEAT_CONTROLLER_H
#define BEAT_CONTROLLER_H

#include "Script.h"
#include "BackgroundMaterial.h"
#include "Prefab.h"

struct BeatController : Script {
	
	Material::Background* background;
	Prefab* indicatorPrefab;

	float threshold;

	void SetTempo(unsigned const& tempo);

private: 
	
	float delay;
	float bt;

	bool isHit; // check if this beat has been hit
	vec3f playerPosition;

	void Awake() override;
	void Start() override;
	void Update() override;
	
	void HitHandler(Events::Event* event);

};

#endif
