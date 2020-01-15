#include "HexScene.h"

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

void HexScene::Awake() {
	Scene::Awake();

	Events::EventsManager::GetInstance()->Subscribe("KEY_INPUT", &HexScene::KeyHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("DRAW_PATH", &HexScene::DrawPath, this);

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

	{
		const unsigned label = entities->Create();
		entities->GetComponent<Transform>(label)->translation.Set(-15.f, 8.f, 0.f);
		debugText = entities->AddComponent<Text>(label);
		debugText->SetFont(font);
		debugText->SetActive(true);
	}

	{
		const unsigned label = entities->Create();
		entities->GetComponent<Transform>(label)->translation.Set(0.f, -9.f, 0.f);
		const auto text = entities->AddComponent<Text>(label);
		text->SetFont(font);
		text->SetActive(true);
		text->scale = 0.5f;
		text->text = "Hold M to spawn Mice.";
	}
	
	maze.Generate(0, gridSize, vec2i(0), 0.7f);

	for (int y = 0; y < gridSize; ++y) {
		for (int x = 0; x < gridSize; ++x) {
			CreateTile(x, y);
		}
	}

	{
		auto highlightTile = entities->Create();

		highlight = entities->GetComponent<Transform>(highlightTile);
		highlight->scale.Set(1.1f);

		auto render = entities->AddComponent<Render>(highlightTile);
		render->SetActive(true);
		render->SetTexture("Files/Textures/hex.tga");
		render->tint.Set(1.f);

		auto animation = entities->AddComponent<Animation>(highlightTile);
		animation->SetActive(true);
	}
}

void HexScene::Update(const float & dt) {
	Scene::Update(dt);

	debugText->text = std::to_string(static_cast<int>(1.f / dt));

	for (auto& mice : mouse) {
		if (!mice.IsDoneExploring()) {
			mice.Explore();
			UpdateVision(mice);
		}
	}
}

void HexScene::KeyHandler(Events::Event * event) {
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

void HexScene::DrawPath(Events::Event * event) {
	auto path = static_cast<Events::AnyType<std::vector<vec2i>>*>(event)->data;

	for (auto& step : path) {
		const auto i = maze.GetMapIndex(step);
		if (i < 0) break;

		grid[i]->tint.a = 1.f;
	}

	//Events::EventsManager::GetInstance()->Trigger("STEP");
}

void HexScene::UpdateVision(const HexMouse& mice) {
	auto vision = mice.GetVision();

	const auto micePosition = mice.GetTransform()->translation.xy;

	for (unsigned i = 0; i < vision.size(); ++i) {
		const auto mapPosition = maze.GetMapPosition(i);
		const auto screenPosition = maze.MapToScreenPosition(mapPosition);

		float a = grid[i]->tint.a;

		if (Math::Length(screenPosition - micePosition) < 3.f) {
			a = (a + 0.5f) * 0.5f;
		} else {
			a = (a + 0.2f) * 0.5f;
		}

		switch (vision[i]) {
		case WALL:
			grid[i]->tint.Set(0.2f, 0.2f, 0.2f, a);
			break;
		case FOG:
			grid[i]->tint.Set(0.f);
			break;
		case PATH:
			grid[i]->tint.Set(0.4f, 0.5f, 0.2f, a);
			break;
		case WATER:
			grid[i]->tint.Set(0.6f, 0.8f, 0.8f, a);
			break;
		case MUD:
			grid[i]->tint.Set(0.6f, 0.2f, 0.2f, a);
			break;
		default:
			break;
		}
	}
}

unsigned HexScene::CreateTile(const int & x, const int & y) {
	auto tile = entities->Create();

	const auto position = maze.MapToScreenPosition(vec2i(x, y));
	entities->GetComponent<Transform>(tile)->translation.Set(
		position,
		0.f
	);
	entities->GetComponent<Transform>(tile)->scale.Set(0.9f);

	auto render = entities->AddComponent<Render>(tile);
	render->SetActive(true);
	render->SetTexture("Files/Textures/hex.tga");

	const int index = maze.GetMapIndex(x, y);
	switch (maze.GetMapData(index)) {
	case WALL:
		render->tint.Set(0.2f, 0.2f, 0.2f, 0.5f);
		break;
	case FOG:
		render->tint.Set(0.f);
		break;
	case PATH:
		render->tint.Set(0.4f, 0.5f, 0.2f, 0.5f);
		break;
	case WATER:
		render->tint.Set(0.6f, 0.8f, 0.8f, 0.5f);
		break;
	case MUD:
		render->tint.Set(0.6f, 0.2f, 0.2f, 0.5f);
		break;
	default:
		break;
	}

	auto button = entities->AddComponent<Button>(tile);
	button->SetActive(true);
	button->BindHandler(MOUSE_OVER, &HexScene::OnMouseOverHandler, this);
	button->BindHandler(MOUSE_OUT, &HexScene::OnMouseOutHandler, this);
	button->BindHandler(MOUSE_DOWN, &HexScene::OnMouseDownHandler, this);
	button->BindHandler(MOUSE_UP, &HexScene::OnMouseUpHandler, this);
	button->BindHandler(MOUSE_CLICK, &HexScene::OnClick, this);

	auto animation = entities->AddComponent<Animation>(tile);
	animation->SetActive(true);

	grid.push_back(render);

	return tile;
}

HexMouse HexScene::CreateMice(const float & x, const float & y) {
	auto entity = entities->Create();

	auto transform = entities->GetComponent<Transform>(entity);
	transform->translation.Set(x, y, 0.1f);
	//transform->scale.Set(0.5f);

	auto render = entities->AddComponent<Render>(entity);
	render->SetActive(true);
	render->tint.Set(0.f, 0.f, 1.f, 1.f);
	render->SetTexture("Files/Textures/hex.tga");

	HexMouse mice = HexMouse(entities->GetComponent<Transform>(entity));
	mice.SetMaze(&maze);
	mice.Explore();
	return mice;
}

void HexScene::OnMouseOverHandler(unsigned entity) {
	//entities->GetComponent<Animation>(entity)->Animate(
	//	AnimationBase(false, 0.2f),
	//	entities->GetComponent<Render>(entity)->tint.a,
	//	1.f
	//);

	auto position = entities->GetComponent<Transform>(entity)->translation;
	highlight->translation = position;
}

void HexScene::OnMouseOutHandler(unsigned entity) {
	//entities->GetComponent<Animation>(entity)->Animate(
	//	AnimationBase(false, 0.2f),
	//	entities->GetComponent<Render>(entity)->tint.a,
	//	0.5f
	//);
}

void HexScene::OnMouseDownHandler(unsigned entity) {
	entities->GetComponent<Animation>(entity)->Animate(
		AnimationBase(false, 0.2f),
		entities->GetComponent<Transform>(entity)->scale,
		vec3f(0.7f)
	);
}

void HexScene::OnMouseUpHandler(unsigned entity) {
	entities->GetComponent<Animation>(entity)->Animate(
		AnimationBase(false, 0.2f),
		entities->GetComponent<Transform>(entity)->scale,
		vec3f(0.9f)
	);
}

void HexScene::OnClick(unsigned entity) {
	const vec3f screenSpace = entities->GetComponent<Transform>(entity)->translation;
	const vec2i mapPosition = maze.ScreenToMapPosition(screenSpace.xy);
	const int index = maze.GetMapIndex(mapPosition);

	if (index < 0) return;

	if (mPressed) {
		mouse.push_back(CreateMice(screenSpace.x, screenSpace.y));
	} else {
		if (!mouse.empty())
			UpdateVision(mouse.front());

		for (auto& mice : mouse) {
			if (mice.IsDoneExploring()) {
				mice.Goto(mapPosition);
			}
		}
	}
}

