#ifndef MAIN_GAME_H
#define MAIN_GAME_H

#include "Scene.h"

// materials
#include "BackgroundMaterial.h"
// prefabs
#include "IndicatorLabel.h"
#include "ExplosionArea.h"
#include "BasicBullet.h"
#include "ExplosiveBullet.h"
#include "BasicEnemy.h"
// weapons
#include "DemoGun.h"

class MainGame : public Scene {

	Material::Background* background;

	IndicatorLabel* indicatorLabel;
	ExplosionArea* explosionArea;
	BasicBullet* basicBullet;
	ExplosiveBullet* explosiveBullet;
	DemoGun* demoGun;

	// enemies
	BasicEnemy* basicEnemy;

private:

	void Awake() override;
	void Create() override;
	void Destroy() override;

};

#endif
