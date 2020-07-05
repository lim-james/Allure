#ifndef PIVOT_SCENE_H
#define PIVOT_SCENE_H

#include "Scene.h"

// materials
#include "CircleMaterial.h"
// prefabs
#include "BeatBullet.h"

class PivotScene : public Scene {
	
	float borderRadius;
	float indicatorRadius;

	Material::Circle* outerCircle;
	Material::Circle* innerCircle;

	BeatBullet* beatBullet;

	void Awake() override;
	void Create() override;
	void Destroy() override;

};

#endif
