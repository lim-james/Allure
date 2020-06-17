#ifndef MAIN_GAME_H
#define MAIN_GAME_H

#include "Scene.h"

// materials
#include "BackgroundMaterial.h"
// prefabs
#include "IndicatorLabel.h"
// bullets
#include "ExplosionArea.h"
#include "BasicBullet.h"
#include "LongBullet.h"
#include "ExplosiveBullet.h"
#include "LaserPath.h"
// guns
#include "Pistol.h"
#include "Automatic.h"
#include "Sniper.h"
#include "Shotgun.h"
#include "GrenadeLauncher.h"
#include "Laser.h"
// enemy prefabs
#include "BasicEnemy.h"
#include "BatEnemy.h"

class MainGame : public Scene {

	Material::Background* background;

	IndicatorLabel* indicatorLabel;
	// projectiles
	ExplosionArea* explosionArea;
	BasicBullet* basicBullet;
	LongBullet* longBullet;
	ExplosiveBullet* explosiveBullet;
	LaserPath* laserPath;
	// guns
	Pistol* pistol;
	Automatic* automatic;
	Shotgun* shotgun;
	Sniper* sniper;
	GrenadeLauncher* grenadeLauncher;
	Laser* laser;
	// enemies
	BasicEnemy* basicEnemy;
	BatEnemy* batEnemy;

	void Awake() override;
	void Create() override;
	void Destroy() override;

};

#endif
