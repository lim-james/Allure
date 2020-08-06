#include "PackTest.h"

#include "Camera.h"
#include "Light2D.h"
#include "TilemapRender.h"
#include "SpriteRender.h"
#include "SpriteAnimationSystem.h"
#include "AnimationSystem.h"
// scripts
#include "CameraFollow.h"
#include "CrosshairController.h"
// utilities
#include "LoadTilemap.h"
#include "LoadTexture.h"

void PackTest::Awake() {
	Scene::Awake();

	systems->Subscribe<SpriteAnimationSystem>(1);
	systems->Subscribe<AnimationSystem>(1);

	// Materials

	spriteLit = new Material::SpriteLit;
	tilemapLit = new Material::TilemapLit;

	circle = new Material::Circle;
	circle->borderWeight = 0.25f;

	// Prefab

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
	camera->SetSize(5.f);
	camera->projection = ORTHOGRAPHIC;
	camera->captureDepth = true;

	CameraFollow* const follow = entities->AddComponent<CameraFollow>(mainCamera);
	follow->SetActive(true);
	follow->offset.z = 10.f;
	follow->stick = false;
	follow->speed = 10.f;
	follow->jutDistance = 1.f;

	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetLocalTranslation(vec3f(-5.f, 6.f, 0.f));

		TilemapRender* const render = entities->AddComponent<TilemapRender>(entity);
		render->SetPalette(Load::TMP("Files/Data/Palettes/world.tmp"));
		render->layout = Load::TML("Files/Data/Levels/levels-(0).csv");
		render->SetActive(true);
		render->SetMaterial(tilemapLit);
	}
	
	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetLocalTranslation(vec3f(-5.f, 7.f, 1.f));

		TilemapRender* const render = entities->AddComponent<TilemapRender>(entity);
		render->SetPalette(Load::TMP("Files/Data/Palettes/world.tmp"));
		render->layout = Load::TML("Files/Data/Levels/levels-(1).csv");
		render->SetActive(true);
		render->SetMaterial(tilemapLit);
	}

	//coinPrefab->Create();

	//slimePrefab->CreateAt(vec3f(1.f, 0.f, 0.f));
	//flyingEnemyPrefab->CreateAt(vec3f(2.f, 0.f, 0.f));
	//skeletonArcherPrefab->CreateAt(vec3f(3.f, 0.f, 0.f));
	//golemMidPrefab->CreateAt(vec3f(-1.f, 0.f, 0.f));
	//golemLittlePrefab->CreateAt(vec3f(-2.f, 0.f, 0.f));

	// crosshair
	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetLocalTranslation(vec3f(0.f, 0.f, 5.f));
		transform->SetScale(0.5f);
		follow->crosshair = transform;

		TransformAnimator* const animator = entities->AddComponent<TransformAnimator>(entity);
		animator->SetActive(true);

		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);
		render->SetMaterial(circle);

		CrosshairController* const controller = entities->AddComponent<CrosshairController>(entity);
		controller->SetActive(true);
		controller->view = camera;
	}

	{
		Transform * const transform = magePrefab->CreateAt(vec3f(-3.f, 0.f, 0.1f));

		follow->player = transform;

		const unsigned entity = transform->entity;

		Light2D* const light = entities->AddComponent<Light2D>(entity);
		light->SetActive(true);
		light->type = LIGHT_POINT;
		light->intensity = 100.f;
		light->strength = 0.5f;
	}

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
