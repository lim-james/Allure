#include "PivotScene.h"

// systems
#include "AudioSystem.h"
#include "PhysicsSystem.h"
#include "ColliderSystem.h"
// components
#include "Camera.h"
#include "Text.h"
#include "SpriteRender.h"
#include "SphereCollider.h"
// scripts
#include "ScreenShake.h"
#include "FPSCounter.h"
#include "CustomCursor.h"
#include "PlayerController.h"
#include "MapCreator.h"
#include "SpawnController.h"
// utils
#include "Layers.h"
#include "ProjectDefines.h"
#include "LoadFNT.h"
#include "LoadTexture.h"

void PivotScene::Awake() {
	Scene::Awake();
	
	systems->Subscribe<AudioSystem>(0);
	systems->Subscribe<PhysicsSystem>(0);
	systems->SubscribeFixed<PhysicsSystem>();
	systems->SubscribeFixed<ColliderSystem>();
	
	borderRadius = 7.5f;
	indicatorRadius = 2.f;

	// materials

	healthCone = new Material::Cone;
	healthCone->borderWeight = 1.f;
	healthCone->SetRange(145.f);

	shieldCone = new Material::Cone;
	shieldCone->borderWeight = 1.5f / (indicatorRadius * 5.f);
	shieldCone->SetRange(30.f);

	cursor = new Material::Circle;
	cursor->borderWeight = 0.2f;

	innerCircle = new Material::Circle;
	innerCircle->borderWeight = 1.f / (indicatorRadius * 10.f);

	outerCircle = new Material::Circle;
	outerCircle->borderWeight = 1.f / (borderRadius * 10.f);

	// prefabs

	beatBullet = new BeatBullet;
	beatBullet->Initialize(entities);
}

void PivotScene::Create() {
	Scene::Create();

	Font* const vcrMono = Load::FNT("Files/Fonts/vcr_ocd_mono.fnt", "Files/Fonts/vcr_ocd_mono.png");

	Camera* const camera = entities->GetComponent<Camera>(mainCamera);
	camera->SetSize(10.f);
	camera->projection = ORTHOGRAPHIC;
	camera->clearColor = COLOR_WHITE;
	camera->cullingMask = DEFAULT | BULLET;

	{
		ScreenShake* const shake = entities->AddComponent<ScreenShake>(mainCamera);
		shake->SetActive(true);
		shake->duration = 0.5f;
		shake->magnitude = 0.025f;
	}

	// FPS counter
	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);

		Text* const text = entities->AddComponent<Text>(entity);
		text->SetActive(true);
		text->SetFont(vcrMono);
		text->scale = 0.5f;

		FPSCounter* const fps = entities->AddComponent<FPSCounter>(entity);
		fps->SetActive(true);
	}

	// player controller
	PlayerController* player = nullptr;
	{
		const unsigned entity = entities->Create();
		
		player = entities->AddComponent<PlayerController>(entity);
		player->SetActive(true);
		player->SetView(camera);
		player->SetMaxHealth(8.f);
	}

	// cursor
	{
		const unsigned entity = entities->Create();
	
		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);
		render->SetMaterial(cursor);
		render->tint = COLOR_LIGHT_GREY;

		CustomCursor* const cursor = entities->AddComponent<CustomCursor>(entity);
		cursor->SetActive(true);
		cursor->view = camera;

		SphereCollider* const collider = entities->AddComponent<SphereCollider>(entity);
		collider->SetActive(true);
	}

	// health bar
	{
		const unsigned entity = entities->Create();
		
		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetLocalRotation(vec3f(0.f, 0.f, -90.f));
		transform->SetScale(vec3f(indicatorRadius * 2.f - 1.f));
	
		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);
		render->SetMaterial(healthCone);
		render->tint = COLOR_BLACK;

		player->SetHealthRender(render);

		SphereCollider* const collider = entities->AddComponent<SphereCollider>(entity);
		collider->SetActive(true);
		collider->handlers[COLLISION_ENTER].Bind(&PlayerController::OnCollisionEnter, player);
	}

	// shield
	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetScale(vec3f(indicatorRadius * 2.1f));

		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);
		render->SetMaterial(shieldCone);
		render->tint = COLOR_GREY;

		player->SetShieldTransform(transform);
	}

	// indicator
	{
		const unsigned entity = entities->Create();
		
		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetLocalTranslation(vec3f(0.f, 0.f, -1.f));
		transform->SetScale(vec3f(indicatorRadius * 2.f));
	
		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);
		render->SetMaterial(innerCircle);
		render->tint = COLOR_LIGHT_GREY;

		player->indicatorRender = render;
	}

	// border
	Transform* borderTransform = nullptr;
	{
		const unsigned entity = entities->Create();

		borderTransform = entities->GetComponent<Transform>(entity);
		borderTransform->SetLocalTranslation(vec3f(0.f, 0.f, -1.f));
		borderTransform->SetScale(vec3f(borderRadius * 2.f));
	
		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);
		render->SetMaterial(outerCircle);
		render->tint = COLOR_LIGHT_GREY;
	}

	//Transform* const transform = beatBullet->Create();
	//transform->SetScale(1.f);

	// map creator
	{
		const unsigned entity = entities->Create();

		AudioSource* const source = entities->AddComponent<AudioSource>(entity);
		source->SetActive(true);
		source->audioClip = "Files/Media/Ken Blast - The Top.wav";
		//source->speed = 1.5f;

		//MapCreator* const creator = entities->AddComponent<MapCreator>(entity);
		//creator->SetActive(true);
		//creator->source = source;
		//creator->savePath = "Files/Data/Maps/Ken Blast - The Top.bm";

		SpawnController* const controller = entities->AddComponent<SpawnController>(entity);
		//controller->SetActive(true);
		controller->source = source;
		controller->bulletPrefab = beatBullet;
		controller->SetOuterRadius(borderRadius);
		controller->SetInnerRadius(indicatorRadius);
		controller->SetTravelTime(1.f);
		controller->SetMapPath("Files/Data/Maps/Ken Blast - The Top.bm");
	}
}

void PivotScene::Destroy() {
	Scene::Destroy();

	delete healthCone;
	delete shieldCone;
	delete cursor;
	delete innerCircle;
	delete outerCircle;

	delete beatBullet;
}
