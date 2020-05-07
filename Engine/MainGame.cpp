#include "MainGame.h"

#include "Camera.h"
#include "SpriteRender.h"
#include "ParticleSystem.h"
#include "PhysicsSystem.h"
#include "ColliderSystem.h"
#include "AudioSystem.h"
// scripts
#include "FPSCounter.h"
#include "CameraFollow.h"
#include "ScreenShake.h"
#include "PlayerMovement.h"
#include "PlayerCombat.h"
#include "CrosshairController.h"
#include "EnemyLife.h"
// weapons
#include "DemoGun.h"
// Utils
#include "Layers.h"
#include "LoadTexture.h"
#include "LoadFNT.h"

void MainGame::Awake() {
	Scene::Awake();
	
	systems->Subscribe<AudioSystem>(0);
	systems->Subscribe<PhysicsSystem>(0);
	systems->Subscribe<ParticleSystem>(1);

	systems->SubscribeFixed<PhysicsSystem>();
	systems->SubscribeFixed<ColliderSystem>();
}

void MainGame::Create() {
	Scene::Create();

	Font* const courierNew = Load::FNT("Files/Fonts/CourierNew.fnt", "Files/Fonts/CourierNew.tga");

	WeaponBase* demoGun = new DemoGun;
	demoGun->Initialize(entities);

	Camera* const camera = entities->GetComponent<Camera>(mainCamera);
	camera->SetSize(10.f);
	camera->projection = ORTHOGRAPHIC;
	camera->cullingMask = DEFAULT | UI | PLAYER | ENEMY | BULLET;

	CameraFollow* const follow = entities->AddComponent<CameraFollow>(mainCamera);
	follow->SetActive(true);
	follow->offset.z = 10.f;
	follow->stick = false;
	follow->speed = 10.f;
	follow->jutDistance = 4.f;

	{
		ScreenShake* const shake = entities->AddComponent<ScreenShake>(mainCamera);
		shake->SetActive(true);
		shake->duration = 0.25f;
		shake->magnitude = 0.075f;

		AudioListener* const listener = entities->AddComponent<AudioListener>(mainCamera);
		listener->SetActive(true);
	}

	// FPS counter
	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->translation = vec3f(-14.f, 8.f, 0.f);

		Text* const text = entities->AddComponent<Text>(entity);
		text->SetActive(true);
		text->SetFont(courierNew);

		FPSCounter* const fps = entities->AddComponent<FPSCounter>(entity);
		fps->SetActive(true);
	}

	// background
	{
		const unsigned entity = entities->Create();

		auto transform = entities->GetComponent<Transform>(entity);
		transform->scale = vec3f(80.0f, 45.0f, 0.0f);

		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);
		render->SetSprite(Load::TGA("Files/Textures/tile.tga"));
		render->SetCellRect(0, 0, 16, 9);
		render->tint.a = 0.15f;
	}

	// crosshair
	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->scale = 0.5f;
		follow->crosshair = transform;

		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);
		render->tint = vec4f(1.f, 0.f, 0.f, 1.f);

		CrosshairController* const controller = entities->AddComponent<CrosshairController>(entity);
		controller->SetActive(true);
		controller->view = camera;
	}

	// player
	{
		const unsigned entity = entities->Create();
		entities->SetLayer(entity, PLAYER);

		follow->player = entities->GetComponent<Transform>(entity);

		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);

		ParticleEmitter* const emitter = entities->AddComponent<ParticleEmitter>(entity);
		emitter->SetActive(true);
		emitter->duration = 0.1f;
		emitter->spawnInterval = 0.1f;
		emitter->lifetime = 0.7f;
		emitter->lifetimeRange = 0.2f;
		emitter->loop = false;
		emitter->speed = 0.f;
		emitter->burstAmount = 15;
		emitter->positionRange.xy = 0.8f;
		emitter->startSize = 0.7f;
		emitter->startSizeRange = 0.3f;
		emitter->endSize = 0.0f;
		emitter->startColor = 0.4f;
		emitter->endColor = 0.4f;

		Physics* const physics = entities->AddComponent<Physics>(entity);
		physics->SetActive(true);
		physics->useGravity = false;
		physics->drag = 10.f;

		SphereCollider* const collider = entities->AddComponent<SphereCollider>(entity);
		collider->SetActive(true);

		AudioSource* const audio = entities->AddComponent<AudioSource>(entity);
		audio->SetActive(true);

		PlayerMovement* const movement = entities->AddComponent<PlayerMovement>(entity);
		movement->SetActive(true);
		movement->speed = 40.f;
		movement->dash = 100.f;

		PlayerCombat* const combat = entities->AddComponent<PlayerCombat>(entity);
		combat->SetActive(true);
		combat->SetCrosshair(follow->crosshair);
		combat->SetWeapon(demoGun);
	}

	// test enemy
	{
		const unsigned entity = entities->Create();
		entities->SetLayer(entity, ENEMY);

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->translation = vec3f(5.f, 0.f, 0.f);

		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);
		render->tint = vec4f(1.f, 0.f, 0.f, 1.f);

		ParticleEmitter* const emitter = entities->AddComponent<ParticleEmitter>(entity);
		emitter->SetActive(true);
		emitter->duration = 0.1f;
		emitter->spawnInterval = 0.1f;
		emitter->lifetime = 1.5f;
		emitter->lifetimeRange = 0.2f;
		emitter->loop = false;
		emitter->speed = 15.f;
		emitter->speedRange = 3.f;
		emitter->drag = 10.f;
		emitter->angleRange.z = 180.f;
		emitter->burstAmount = 30;
		emitter->positionRange.xy = 0.0f;
		emitter->startSize = 0.7f;
		emitter->startSizeRange = 0.3f;
		emitter->endSize = 0.0f;
		emitter->startColor = vec4f(1.f, 0.f, 0.f, 1.f);
		emitter->endColor = vec4f(1.f, 0.f, 0.f, 1.f);

		Physics* const physics = entities->AddComponent<Physics>(entity);
		physics->SetActive(true);
		physics->useGravity = false;
		physics->drag = 10.f;

		SphereCollider* const collider = entities->AddComponent<SphereCollider>(entity);
		collider->SetActive(true);

		EnemyLife* const life = entities->AddComponent<EnemyLife>(entity);
		life->SetActive(true);
		collider->handlers[COLLISION_ENTER].Bind(&EnemyLife::OnCollisionEnter, life);
	}
}
