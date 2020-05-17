#ifndef BEAT_CONTROLLER_H
#define BEAT_CONTROLLER_H

#include "Script.h"
#include "BackgroundMaterial.h"
#include "Prefab.h"

struct BeatController : Script {

	Material::Background* material;
	Prefab* indicatorPrefab;

	float threshold;

	void SetTempo(unsigned const& tempo);
	void Hit();

private:

	float delay;
	float bt;

	bool isHit;

	void Start();
	void Update();

};

#endif
