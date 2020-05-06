#include "MainGame.h"

#include "Camera.h"
#include "SpriteRender.h"
#include "PhysicsSystem.h"
// scripts
#include "CameraFollow.h"
#include "PlayerInput.h"
#include "CrosshairController.h"
// Utils
#include "LoadTexture.h"

void MainGame::Awake() {
	Scene::Awake();
	
	systems->SubscribeFixed<PhysicsSystem>();
}

void MainGame::Create() {
	Scene::Create();

	Camera* const camera = entities->GetComponent<Camera>(mainCamera);
	camera->SetSize(10.f);
	camera->projection = ORTHOGRAPHIC;

	CameraFollow* const follow = entities->AddComponent<CameraFollow>(mainCamera);
	follow->SetActive(true);
	follow->offset.z = 10.f;
	follow->stick = false;
	follow->speed = 10.f;
	follow->jutDistance = 4.f;

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

	// player
	{
		const unsigned entity = entities->Create();

		follow->player = entities->GetComponent<Transform>(entity);

		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);

		Physics* const physics = entities->AddComponent<Physics>(entity);
		physics->SetActive(true);
		physics->useGravity = false;
		physics->drag = 10.f;

		PlayerInput* const input = entities->AddComponent<PlayerInput>(entity);
		input->SetActive(true);
		input->speed = 20.f;
	}

	// crosshair
	{
		const unsigned entity = entities->Create();

		follow->crosshair = entities->GetComponent<Transform>(entity);

		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);
		render->tint = vec4f(1.f, 0.f, 0.f, 1.f);

		CrosshairController* const controller = entities->AddComponent<CrosshairController>(entity);
		controller->SetActive(true);
		controller->view = camera;
	}
}
