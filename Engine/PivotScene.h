#ifndef PIVOT_SCENE_H
#define PIVOT_SCENE_H

#include "Scene.h"

// materials
#include "CircleMaterial.h"
#include "ConeMaterial.h"
// prefabs
#include "BeatBullet.h"

class PivotScene : public Scene {
	
	float borderRadius;
	float indicatorRadius;

	Material::Cone* healthCone;
	Material::Cone* shieldCone;
	Material::Circle* cursor;
	Material::Circle* innerCircle;
	Material::Circle* outerCircle;
	Material::Circle* bulletCircle;

	BeatBullet* beatBullet;

	void Awake() override;
	void Create() override;
	void Destroy() override;

};

#endif
