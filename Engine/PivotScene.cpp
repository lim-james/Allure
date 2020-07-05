#include "PivotScene.h"

// systems
#include "AudioSystem.h"
#include "PhysicsSystem.h"
#include "ColliderSystem.h"
// components
#include "Camera.h"
#include "SpriteRender.h"
#include "SphereCollider.h"
// scripts
#include "MapCreator.h"
#include "SpawnController.h"
// utils
#include "Layers.h"
#include "LoadFNT.h"
#include "LoadTexture.h"

void PivotScene::Awake() {
	Scene::Awake();
	
	systems->Subscribe<AudioSystem>(0);
	systems->Subscribe<PhysicsSystem>(0);
	systems->SubscribeFixed<PhysicsSystem>();
	systems->SubscribeFixed<ColliderSystem>();
	
	borderRadius = 9.f;
	indicatorRadius = 2.5f;

	// materials

	outerCircle = new Material::Circle;
	outerCircle->borderWeight = 1.f / (borderRadius * 20.f);

	innerCircle = new Material::Circle;
	innerCircle->borderWeight = 1.f / (indicatorRadius * 20.f);

	// prefabs

	beatBullet = new BeatBullet;
	beatBullet->Initialize(entities);
}

void PivotScene::Create() {
	Scene::Create();

	Camera* const camera = entities->GetComponent<Camera>(mainCamera);
	camera->SetSize(10.f);
	camera->projection = ORTHOGRAPHIC;
	camera->clearColor = vec4f(0.f);
	camera->cullingMask = DEFAULT | BULLET;

	// border
	Transform* borderTransform = nullptr;
	{
		const unsigned entity = entities->Create();

		borderTransform = entities->GetComponent<Transform>(entity);
		borderTransform->SetLocalTranslation(vec3f(0.f, 0.f, -5.f));
		borderTransform->SetScale(vec3f(borderRadius * 2.f));
	
		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);
		render->SetMaterial(outerCircle);
	}

	// player
	{
		const unsigned entity = entities->Create();
		
		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetLocalTranslation(vec3f(0.f, 0.f, -4.f));
		transform->SetScale(vec3f(indicatorRadius * 2.f));
	
		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);
		render->SetMaterial(innerCircle);
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
		controller->SetActive(true);
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

	delete outerCircle;
	delete innerCircle;

	delete beatBullet;
}
