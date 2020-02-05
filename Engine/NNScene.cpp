#include "NNScene.h"

// components
#include "Transform.h"
#include "Render.h"
#include "Camera.h"
// systems
#include "PhysicsSystem.h"
#include "ColliderSystem.h"
// utils
#include "LoadTGA.h"
#include "InputEvents.h"

#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void NNScene::Awake() {
	Scene::Awake();

	// initializing local variables

	vertSpeed = 15.f;
	gameSpeed = 2.f;

	// subscribe systems
	systems->Subscribe<PhysicsSystem>(1);
	systems->Subscribe<ColliderSystem>(2);

	// set up scene
	
	auto camera = entities->GetComponent<Camera>(mainCamera);
	camera->clearColor.Set(113.f / 255.f, 197.f / 255.f, 207.f / 255.f, 1.f);
	camera->SetSize(8);

	// loading assets
	const unsigned sprites = Load::TGA("Files/Textures/flappy_bird.tga");

	// creating scene

	// background
	//{
	//	const unsigned bg = entities->Create();
	//	
	//	auto transform = entities->GetComponent<Transform>(bg);
	//	transform->scale.Set(9.f, 16.f, 1.f);
	//	
	//	auto render = entities->AddComponent<Render>(bg);
	//	//render->SetActive(true);
	//	render->SetTexture(sprites);
	//	render->SetTilemapSize(453, 256);
	//	render->SetCellRect(0, 0, 144, 256);
	//}

	// ground
	{
		const unsigned gr = entities->Create();
		
		auto transform = entities->GetComponent<Transform>(gr);
		transform->translation.y = -6.5f;
		transform->scale.Set(9.f, 3.f, 1.f);
		
		auto render = entities->AddComponent<Render>(gr);
		render->SetActive(true);
		render->tint.Set(224.f / 255.f, 215.f / 255.f, 150.f / 255.f, 1.f);

		auto collider = entities->AddComponent<Collider>(gr);
		collider->SetActive(true);
		collider->BindHandler(COLLIDER_ENTER, &NNScene::CollisionHandler, this);
		collider->tag = "ENV";
		collider->ignore.push_back("ENV");
	}

	// bird
	{
		const unsigned bird = entities->Create();
		
		auto transform = entities->GetComponent<Transform>(bird);
		transform->scale.Set(0.5f);
		
		auto render = entities->AddComponent<Render>(bird);
		render->SetActive(true);
		render->tint.Set(1.f, 1.f, 0.f, 1.f);

		birdPhysics = entities->AddComponent<Physics>(bird);
		birdPhysics->SetActive(true);

		auto collider = entities->AddComponent<Collider>(bird);
		collider->SetActive(true);
	}

	// event subscriptions
	Events::EventsManager::GetInstance()->Subscribe("KEY_INPUT", &NNScene::KeyHandler, this);

	CreatePipe(8.f, 0.f);
	CreatePipe(12.f, 1.f);
	CreatePipe(16.f, 2.f);
	CreatePipe(20.f, 1.f);
}

void NNScene::Update(const float& dt) {
	Scene::Update(dt);

	for (auto& pipe : pipes) {
		auto t1 = entities->GetComponent<Transform>(pipe.first);
		auto t2 = entities->GetComponent<Transform>(pipe.second);
		t1->translation.x -= dt * gameSpeed;
		t2->translation.x -= dt * gameSpeed;

		if (t1->translation.x < -8.f) {
			t1->translation.x = 8.f;
			t2->translation.x = 8.f;
		}
	}
}


void NNScene::KeyHandler(Events::Event * event) {
	auto input = static_cast<Events::KeyInput*>(event);
	
	if (input->key == GLFW_KEY_SPACE && input->action == GLFW_PRESS) {
		//birdPhysics->velocity.y = Math::Abs(birdPhysics->velocity.y) + 2.f;
		birdPhysics->velocity.y = vertSpeed;
	}
}

void NNScene::CollisionHandler(unsigned target) {
	ResetGame();
	//entities->GetComponent<Transform>(target)->translation.Set(0.f);
	//entities->GetComponent<Physics>(target)->velocity.Set(0.f);
}

void NNScene::CreatePipe(const float& x, const float& openingY) {
	std::pair<unsigned, unsigned> pair;

	{
		const unsigned pipe = entities->Create();
		pair.first = pipe;

		auto transform = entities->GetComponent<Transform>(pipe);
		transform->translation.Set(x, openingY + 6.f, 0.f);
		transform->scale.Set(1.f, 7.f, 1.f);

		auto render = entities->AddComponent<Render>(pipe);
		render->SetActive(true);
		render->tint.Set(113.f / 255.f, 191.f / 255.f, 46.f / 255.f, 1.f);

		auto collider = entities->AddComponent<Collider>(pipe);
		collider->SetActive(true);
		collider->BindHandler(COLLIDER_ENTER, &NNScene::CollisionHandler, this);
		collider->tag = "ENV";
		collider->ignore.push_back("ENV");

	}

	{
		const unsigned pipe = entities->Create();
		pair.second = pipe;

		auto transform = entities->GetComponent<Transform>(pipe);
		transform->translation.Set(x, openingY - 6.f, 0.f);
		transform->scale.Set(1.f, 7.f, 1.f);

		auto render = entities->AddComponent<Render>(pipe);
		render->SetActive(true);
		render->tint.Set(113.f / 255.f, 191.f / 255.f, 46.f / 255.f, 1.f);

		auto collider = entities->AddComponent<Collider>(pipe);
		collider->SetActive(true);
		collider->BindHandler(COLLIDER_ENTER, &NNScene::CollisionHandler, this);
		collider->tag = "ENV";
		collider->ignore.push_back("ENV");
	}

	pipes.push_back(pair);
}

void NNScene::ResetGame() {
	// reset bird
	entities->GetComponent<Transform>(birdPhysics->entity)->translation.Set(0.f);
	birdPhysics->velocity.Set(0.f);

	// reset pipes
	for (auto& pipe : pipes) {
		entities->Destroy(pipe.first);
		entities->Destroy(pipe.second);
	}

	CreatePipe(8.f, 0.f);
	CreatePipe(12.f, 1.f);
	CreatePipe(16.f, 2.f);
	CreatePipe(20.f, 1.f);
}
