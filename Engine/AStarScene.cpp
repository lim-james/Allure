#include "AStarScene.h"

// components
#include "Transform.h"
#include "Camera.h"
// systems
#include "RenderSystem.h"
#include "ButtonSystem.h"
#include "AnimationSystem.h"

#include "LoadFNT.h"

#include "InputEvents.h"

#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void AStarScene::Awake() {
	Scene::Awake();

	Events::EventsManager::GetInstance()->Subscribe("KEY_INPUT", &AStarScene::KeyHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("DRAW_PATH", &AStarScene::DrawPath, this);

	gridSize = 12;

	auto font = Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga");

	systems->Subscribe<RenderSystem>(0);
	systems->Subscribe<ButtonSystem>(1);
	systems->Subscribe<AnimationSystem>(2);

	const unsigned cam = entities->Create();
	entities->GetComponent<Transform>(cam)->translation.z = 1.f;
	camera = entities->AddComponent<Camera>(cam);
	camera->SetSize(10);
	camera->SetActive(true);
	camera->clearColor.Set(0.f);

	const unsigned label = entities->Create();
	entities->GetComponent<Transform>(label)->translation.Set(-15.f, 8.f, 0.f);
	debugText = entities->AddComponent<Text>(label);
	debugText->SetFont(font);
	debugText->SetActive(true);

	maze.Generate(0, gridSize, vec2i(0), 0.3f);

	//for (auto& mice : mouse) {
	//	mice = entities->Create<Mouse>();
	//	mice->SetMaze(&maze);
	//	mice->SetMapPosition(0.f);
	//	mice->GetComponent<Transform>()->scale.Set(1.f);
	//	mice->GetComponent<Render>()->tint.Set(0.f, 0.f, 1.f, 1.f);
	//	mice->Explore();
	//}

	const float fgs = static_cast<float>(gridSize) * 0.5f;
	for (float x = -fgs; x < fgs; ++x) {
		for (float y = -fgs; y < fgs; ++y) {
			CreateTile(x, y);
		}
	}
}

void AStarScene::Update(const float & dt) {
	Scene::Update(dt);

	debugText->text = std::to_string(static_cast<int>(1.f / dt));

	for (auto& mice : mouse) {
		if (!mice.IsDoneExploring()) {
			mice.Explore();
		}
	}
}

void AStarScene::KeyHandler(Events::Event * event) {
	Events::KeyInput* input = static_cast<Events::KeyInput*>(event);

	if (input->key == GLFW_KEY_M) {
		if (input->action == GLFW_PRESS) {
			mPressed = true;
		}

		if (input->action == GLFW_RELEASE) {
			mPressed = false;
		}
	}
}

void AStarScene::DrawPath(Events::Event * event) {
	auto path = static_cast<Events::AnyType<std::vector<vec2i>>*>(event)->data;

	for (auto& step : path) {
		const auto i = maze.GetMapIndex(step);
		if (i < 0) break;

		grid[i]->tint.Set(0.f, 1.f, 1.f, 1.f);
	}

	//Events::EventsManager::GetInstance()->Trigger("STEP");
}

void AStarScene::UpdateVision(const Mouse& mice) {
	auto vision = mice.GetVision();

	for (unsigned i = 0; i < vision.size(); ++i) {
		switch (vision[i]) {
		case WALL:
			grid[i]->tint.Set(1.f, 0.f, 0.f, 0.5f);
			break;
		case FOG:
			grid[i]->tint.Set(0.f);
			break;
		case PATH:
			grid[i]->tint.Set(1.f, 1.f, 1.f, 0.5f);
			break;
		default:
			break;
		}
	}
}

unsigned AStarScene::CreateTile(const float & x, const float & y) {
	auto tile = entities->Create();
	entities->GetComponent<Transform>(tile)->translation.Set(
		x, y,
		//x * 0.75f + 0.5f, 
		//y * 0.866f + static_cast<int>(abs(x)) % 2 * 0.433f,  
		0.f
	);
	entities->GetComponent<Transform>(tile)->scale.Set(0.9f);

	auto render = entities->AddComponent<Render>(tile);
	render->SetActive(true);
	//render->SetTexture("Files/Textures/hex.tga");

	const auto position = maze.ScreenToMapPosition(vec2f(x, y));
	const int index = maze.GetMapIndex(position);
	switch (maze.GetMapData(index)) {
	case WALL:
		render->tint.Set(1.f, 0.f, 0.f, 0.5f);
		break;
	case FOG:
		render->tint.Set(0.f);
		break;
	case PATH:
		render->tint.Set(1.f, 1.f, 1.f, 0.5f);
		break;
	default:
		break;
	}

	auto button = entities->AddComponent<Button>(tile);
	button->SetActive(true);
	button->BindHandler(MOUSE_OVER, &AStarScene::OnMouseOverHandler, this);
	button->BindHandler(MOUSE_OUT, &AStarScene::OnMouseOutHandler, this);
	button->BindHandler(MOUSE_DOWN, &AStarScene::OnMouseDownHandler, this);
	button->BindHandler(MOUSE_UP, &AStarScene::OnMouseUpHandler, this);
	button->BindHandler(MOUSE_CLICK, &AStarScene::OnClick, this);

	auto animation = entities->AddComponent<Animation>(tile);
	animation->SetActive(true);

	grid.push_back(render);

	return tile;
}

Mouse AStarScene::CreateMice(const float & x, const float & y) {
	auto entity = entities->Create();

	auto transform = entities->GetComponent<Transform>(entity);
	transform->translation.Set(x, y, 0.1f);
	//transform->scale.Set(0.5f);

	auto render = entities->AddComponent<Render>(entity);
	render->SetActive(true);
	render->tint.Set(0.f, 0.f, 1.f, 1.f);
	//render->SetTexture("Files/Textures/circle.tga");

	Mouse mice = Mouse(entities->GetComponent<Transform>(entity));
	mice.SetMaze(&maze);
	mice.Explore();
	return mice;
}

void AStarScene::OnMouseOverHandler(unsigned entity) {
	entities->GetComponent<Animation>(entity)->Animate(
		AnimationBase(false, 0.2f),
		entities->GetComponent<Render>(entity)->tint.a,
		1.f
	);
}

void AStarScene::OnMouseOutHandler(unsigned entity) {
	entities->GetComponent<Animation>(entity)->Animate(
		AnimationBase(false, 0.2f),
		entities->GetComponent<Render>(entity)->tint.a,
		0.5f
	);
}

void AStarScene::OnMouseDownHandler(unsigned entity) {
	entities->GetComponent<Animation>(entity)->Animate(
		AnimationBase(false, 0.2f),
		entities->GetComponent<Transform>(entity)->scale,
		vec3f(0.5f)
	);
}

void AStarScene::OnMouseUpHandler(unsigned entity) {
	entities->GetComponent<Animation>(entity)->Animate(
		AnimationBase(false, 0.2f),
		entities->GetComponent<Transform>(entity)->scale,
		vec3f(0.9f)
	);
}

void AStarScene::OnClick(unsigned entity) {
	const vec3f screenSpace = entities->GetComponent<Transform>(entity)->translation;
	const vec2i mapPosition = maze.ScreenToMapPosition(screenSpace.xy);
	const int index = maze.GetMapIndex(mapPosition);

	if (index < 0) return;

	if (mPressed) {
		mouse.push_back(CreateMice(screenSpace.x, screenSpace.y));
	} else {
		UpdateVision(mouse.front());

		for (auto& mice : mouse) {
			if (mice.IsDoneExploring()) {
				mice.Goto(mapPosition);
			}
		}
	}
}

