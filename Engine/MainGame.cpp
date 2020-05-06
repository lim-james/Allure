#include "MainGame.h"

#include "Camera.h"
#include "SpriteRender.h"
#include "PhysicsSystem.h"
// scripts
#include "FPSCounter.h"
#include "CameraFollow.h"
#include "PlayerInput.h"
#include "CrosshairController.h"
// Utils
#include "LoadTexture.h"
#include "LoadFNT.h"

void MainGame::Awake() {
	Scene::Awake();
	
	systems->SubscribeFixed<PhysicsSystem>();
}

void MainGame::Create() {
	Scene::Create();

	Font* const courierNew = Load::FNT("Files/Fonts/CourierNew.fnt", "Files/Fonts/CourierNew.tga");

	Camera* const camera = entities->GetComponent<Camera>(mainCamera);
	camera->SetSize(10.f);
	camera->projection = ORTHOGRAPHIC;

	CameraFollow* const follow = entities->AddComponent<CameraFollow>(mainCamera);
	follow->SetActive(true);
	follow->offset.z = 10.f;
	follow->stick = false;
	follow->speed = 10.f;
	follow->jutDistance = 4.f;

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
		input->speed = 40.f;
		input->dash = 100.f;
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
}
