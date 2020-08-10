#ifndef MAIN_GAME_H
#define MAIN_GAME_H

#include "Scene.h"

// materials
#include "BackgroundMaterial.h"
#include "SpriteDefaultMaterial.h"
#include "CircleMaterial.h"
// prefabs
#include "IndicatorLabel.h"
#include "SFXEmitter.h"
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
#include "FireElementalEnemy.h"
#include "IceElementalEnemy.h"
#include "CyclopsEnemy.h"
// song
#include "SongData.h"
#include "MapData.h"

class MainGame : public Scene {

	Material::Background* background;
	Material::SpriteDefault* transparentSprite;
	Material::Circle* circle;

	IndicatorLabel* indicatorLabel;
	SFXEmitter* sfxEmitter;
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
	FireElementalEnemy* fireElementalEnemy;
	IceElementalEnemy* iceElementalEnemy;
	CyclopsEnemy* cyclopsEnemy;

public:

	//SongData song;
	std::string mapPath;
	MapData mapData;

private:

	void Awake() override;
	void Create() override;
	void Destroy() override;

};

#endif
