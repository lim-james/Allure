#ifndef BEAT_CONTROLLER_H
#define BEAT_CONTROLLER_H

#include "Script.h"
#include "BackgroundMaterial.h"

struct BeatController : Script {

	Material::Background* material;

	float threshold;

	void SetTempo(unsigned const& tempo);
	void Hit();

private:

	float delay;
	float bt;

	void Start();
	void Update();

};

#endif
