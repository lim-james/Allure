#ifndef PACK_TEST_H
#define PACK_TEST_H

#include "Scene.h"

// Prefabs
#include "CoinPrefab.h"
#include "FlyingEnemyPrefab.h"
#include "SkeletonArcherPrefab.h"
#include "SlimePrefab.h"
#include "GolemMidPrefab.h"
#include "GolemLittlePrefab.h"
#include "MagePrefab.h"

#include "SpawnerPrefab.h"
#include "StoneShieldPrefab.h"
#include "CastingCirclePrefab.h"

#include "SpikesPrefab.h"
#include "ChestPrefab.h"

#include "AltarCandlePrefab.h"
#include "Candle1Prefab.h"
#include "Candle2Prefab.h"

class PackTest : public Scene {

	CoinPrefab* coinPrefab;
	FlyingEnemyPrefab* flyingEnemyPrefab;
	SkeletonArcherPrefab* skeletonArcherPrefab;
	SlimePrefab* slimePrefab;
	GolemMidPrefab* golemMidPrefab;
	GolemLittlePrefab* golemLittlePrefab;
	MagePrefab* magePrefab;

	SpawnerPrefab* spawnerPrefab;
	StoneShieldPrefab* stoneShieldPrefab;
	CastingCirclePrefab* castingCirclePrefab;

	SpikesPrefab* spikesPrefab;
	ChestPrefab* chestPrefab;

	AltarCandlePrefab* altarCandlePrefab;
	Candle1Prefab* candle1Prefab;
	Candle2Prefab* candle2Prefab;

	void Awake() override;
	void Create() override;
	void Destroy() override;

};

#endif
