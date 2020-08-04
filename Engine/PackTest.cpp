#include "PackTest.h"

#include "Camera.h"
#include "TilemapRender.h"
#include "SpriteRender.h"
#include "SpriteAnimationSystem.h"
// utilities
#include "LoadTilemap.h"
#include "LoadTexture.h"

void PackTest::Awake() {
	Scene::Awake();

	systems->Subscribe<SpriteAnimationSystem>(1);

	coinPrefab = new CoinPrefab;
	coinPrefab->Initialize(entities);

	flyingEnemyPrefab = new FlyingEnemyPrefab;
	flyingEnemyPrefab->Initialize(entities);

	skeletonArcherPrefab = new SkeletonArcherPrefab;
	skeletonArcherPrefab->Initialize(entities);

	slimePrefab = new SlimePrefab;
	slimePrefab->Initialize(entities);

	golemMidPrefab = new GolemMidPrefab;
	golemMidPrefab->Initialize(entities);

	golemLittlePrefab = new GolemLittlePrefab;
	golemLittlePrefab->Initialize(entities);

	magePrefab = new MagePrefab;
	magePrefab->Initialize(entities);

	spawnerPrefab = new SpawnerPrefab;
	spawnerPrefab->Initialize(entities);

	stoneShieldPrefab = new StoneShieldPrefab;
	stoneShieldPrefab->Initialize(entities);

	castingCirclePrefab = new CastingCirclePrefab;
	castingCirclePrefab->Initialize(entities);

	spikesPrefab = new SpikesPrefab;
	spikesPrefab->Initialize(entities);

	chestPrefab = new ChestPrefab;
	chestPrefab->Initialize(entities);

	altarCandlePrefab = new AltarCandlePrefab;
	altarCandlePrefab->Initialize(entities);

	candle1Prefab = new Candle1Prefab;
	candle1Prefab->Initialize(entities);

	candle2Prefab = new Candle2Prefab;
	candle2Prefab->Initialize(entities);
}

void PackTest::Create() {
	Scene::Create();

	Camera* const camera = entities->GetComponent<Camera>(mainCamera);
	camera->clearColor = 1.f;

	{
		const unsigned entity = entities->Create();

		TilemapRender* const render = entities->AddComponent<TilemapRender>(entity);
		render->SetPalette(Load::TMP("Files/Data/Palettes/world.tmp"));
		render->layout = Load::TML("Files/Data/Levels/level-walls.csv");
		render->SetActive(true);
	}
	
	//coinPrefab->Create();

	//slimePrefab->CreateAt(vec3f(1.f, 0.f, 0.f));
	//flyingEnemyPrefab->CreateAt(vec3f(2.f, 0.f, 0.f));
	//skeletonArcherPrefab->CreateAt(vec3f(3.f, 0.f, 0.f));
	//golemMidPrefab->CreateAt(vec3f(-1.f, 0.f, 0.f));
	//golemLittlePrefab->CreateAt(vec3f(-2.f, 0.f, 0.f));
	//magePrefab->CreateAt(vec3f(-3.f, 0.f, 0.f));

	//spawnerPrefab->CreateAt(vec3f(0.f, -1.f, 0.f));
	//stoneShieldPrefab->CreateAt(vec3f(-1.f, -1.f, 0.f));
	//castingCirclePrefab->CreateAt(vec3f(-3.f, -1.f, 0.f));

	//spikesPrefab->CreateAt(vec3f(0.f, -2.f, 0.f));
	//chestPrefab->CreateAt(vec3f(1.f, -2.f, 0.f));

	//altarCandlePrefab->CreateAt(vec3f(0.f, -3.f, 0.f));
	//candle1Prefab->CreateAt(vec3f(1.f, -3.f, 0.f));
	//candle2Prefab->CreateAt(vec3f(2.f, -3.f, 0.f));
}

void PackTest::Destroy() {
	Scene::Destroy();
	delete coinPrefab;
	delete flyingEnemyPrefab;
	delete skeletonArcherPrefab;
	delete slimePrefab;
	delete golemMidPrefab;
	delete golemLittlePrefab;
	delete magePrefab;
	delete spawnerPrefab;
	delete stoneShieldPrefab;
	delete castingCirclePrefab;
	delete spikesPrefab;
	delete chestPrefab;
	delete altarCandlePrefab;
	delete candle1Prefab;
	delete candle2Prefab;
}
