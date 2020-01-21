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
#include <Math/Math.hpp>
#include <GLFW/glfw3.h>

void HexScene::Awake() {
	Scene::Awake();

	moveDelay = 0.2f;
	bt = 0.f;

	Events::EventsManager::GetInstance()->Subscribe("KEY_INPUT", &HexScene::KeyHandler, this);

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
		promptText = entities->AddComponent<Text>(label);
		promptText->SetFont(font);
		promptText->SetActive(true);
		promptText->scale = 0.5f;
	}
	
	maze = new HexMaze(gridSize, PATH);
	maze->Generate(0, vec2i(0), 0.7f);

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

	maxMoves = 10;
	moveCount = 0;
	turnCount = 0;

	teams[0].SetVision(gridSize);
	teams[0].SetMaze(maze);
	teams[1].SetVision(gridSize);
	teams[1].SetMaze(maze);

	teams[0].AddUnit(CreateUnit(1, 1, vec4f(1.f, 1.f, 0.f, 1.f), 4.f));
	teams[0].AddUnit(CreateUnit(3, 3, vec4f(1.f, 1.f, 0.f, 1.f), 10.f));
	teams[1].AddUnit(CreateUnit(10, 10, vec4f(0.f, 1.f, 1.f, 1.f), 4.f));
	teams[1].AddUnit(CreateUnit(8, 8, vec4f(0.f, 1.f, 1.f, 1.f), 4.f));

	playerTurn = false;

	UpdateVision();
}

void HexScene::Update(const float & dt) {
	Scene::Update(dt);

	debugText->text = std::to_string(static_cast<int>(1.f / dt));

	bt += dt;
	if (bt > moveDelay) {
		if (teams[playerTurn].Move(moveDelay, entities)) {
			UpdateVision();
			bt = 0.f;
		} else if (moveCount >= GetCurrentMaxMoves()) {
			Console::Warn << "Changed turn\n";
			playerTurn = !playerTurn;
			moveCount = 0;
			++turnCount;
			UpdateVision();
		}
	}
}

void HexScene::KeyHandler(Events::Event * event) {
	Events::KeyInput* input = static_cast<Events::KeyInput*>(event);

	if (input->key == GLFW_KEY_SPACE) {
		if (input->action == GLFW_PRESS) {
			spacePressed = true;
			auto selected = teams[playerTurn].GetSelectedUnit();
			if (selected) {
				UpdateVision();
				DrawView(selected);
			}
		}

		if (input->action == GLFW_RELEASE) {
			spacePressed = false;
			UpdateVision();
			DrawPath();
		}
	}
}

void HexScene::UpdateVision() {
	auto vision = teams[playerTurn].GetVision();

	const float a = 0.5f;

	for (auto& unit : teams[playerTurn].GetUnits()) {
		entities->GetComponent<Render>(unit->transform->entity)->SetActive(true);
	}

	std::map<unsigned, Render*> otherUnits;
	for (auto& unit : teams[!playerTurn].GetUnits()) {
		const auto mapPosition = vision->ScreenToMapPosition(unit->transform->translation.xy);
		auto render = entities->GetComponent<Render>(unit->transform->entity);
		render->SetActive(false);
		otherUnits[vision->GetMapIndex(mapPosition)] = render;
	}

	for (unsigned i = 0; i < vision->GetSize() * vision->GetSize(); ++i) {
		const auto mapPosition = vision->GetMapPosition(i);
		const auto screenPosition = vision->MapToScreenPosition(mapPosition);
		const auto tile = vision->GetMapData(i);

		entities->GetComponent<Animation>(grid[i]->entity)->Animate(
			AnimationBase(false, 0.2f),
			grid[i]->tint,
			maze->GetColour(tile)
		);

		if (tile != FOG && otherUnits[i]) {
			otherUnits[i]->SetActive(true);
		}
	}
}

void HexScene::Highlight(Render * const tile) {
	//tile->tint = (tile->tint + vec4f(1.f, 1.f, 0.f, 1.f)) * 0.5f;
	auto color = tile->tint; Math::Mix(tile->tint, vec4f(1.f, 1.f, 1.f, 1.f), 0.9f);
	color.a = 1.f;

	entities->GetComponent<Animation>(tile->entity)->Animate(
		AnimationBase(false, 0.2f),
		tile->tint,
		color
	);
}

void HexScene::DrawPath() {
	for (auto& pos : path) {
		//grid[maze->GetMapIndex(pos)]->tint.a = 1.f;
		Highlight(grid[maze->GetMapIndex(pos)]);
	}
}

void HexScene::DrawView(Unit * const unit) {
	for (auto& index : unit->vision) {
		//grid[index]->tint.a = 1.f;
		Highlight(grid[index]);
	}
}

unsigned HexScene::CreateTile(const int & x, const int & y) {
	auto tile = entities->Create();

	const auto position = maze->MapToScreenPosition(vec2i(x, y));
	entities->GetComponent<Transform>(tile)->translation.Set(
		position,
		0.f
	);
	entities->GetComponent<Transform>(tile)->scale.Set(0.9f);

	auto render = entities->AddComponent<Render>(tile);
	render->SetActive(true);
	render->SetTexture("Files/Textures/hex.tga");

	const int index = maze->GetMapIndex(x, y);
	render->tint = maze->GetColour(maze->GetMapData(index));

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

Unit * HexScene::CreateUnit(const int & x, const int & y, const vec4f& color, const float& range) {
	auto entity = entities->Create();

	auto transform = entities->GetComponent<Transform>(entity);
	transform->translation.Set(maze->MapToScreenPosition(vec2i(x, y)), 0.1f);
	transform->scale.Set(0.5f);

	auto render = entities->AddComponent<Render>(entity);
	render->SetActive(true);
	render->tint = color;
	render->SetTexture("files/textures/circle.tga");

	auto button = entities->AddComponent<Button>(entity);
	button->SetActive(true);
	button->BindHandler(MOUSE_OVER, &HexScene::UnitEnterHandler, this);
	button->BindHandler(MOUSE_OUT, &HexScene::UnitExitHandler, this);
	button->BindHandler(MOUSE_CLICK, &HexScene::SelectUnitHandler, this);

	auto animation = entities->AddComponent<Animation>(entity);
	animation->SetActive(true);

	Unit* unit = new Unit;
	unit->transform = transform;
	unit->range = range;

	return unit;
}

//HexMouse HexScene::CreateMice(const float & x, const float & y) {
//	auto entity = entities->Create();
//
//	auto transform = entities->GetComponent<Transform>(entity);
//	transform->translation.Set(x, y, 0.1f);
//	//transform->scale.Set(0.5f);
//
//	auto render = entities->AddComponent<Render>(entity);
//	render->SetActive(true);
//	render->tint.Set(0.f, 0.f, 1.f, 1.f);
//	render->SetTexture("Files/Textures/hex.tga");
//
//	HexMouse mice = HexMouse(entities->GetComponent<Transform>(entity));
//	mice.SetMaze(&maze);
//	mice.Explore();
//	UpdateVision(mice);
//	return mice;
//}

void HexScene::OnMouseOverHandler(unsigned entity) {
	auto position = entities->GetComponent<Transform>(entity)->translation;
	highlight->translation = position;

	auto& team = teams[playerTurn];
	auto selected = team.GetSelectedUnit();
	const auto max = GetCurrentMaxMoves();

	if (selected && team.InSight(position) && moveCount < max) {
		auto vision = team.GetVision();
		auto index = vision->GetMapIndex(vision->ScreenToMapPosition(position));

		if (vision->GetMapData(index) == WALL)
			return;

		path = vision->GetPath(selected->transform->translation, position);
		const unsigned movesLeft = max - moveCount;
		if (path.size() > movesLeft) {
			path.erase(path.begin() + movesLeft, path.end());
		}

		if (!spacePressed) {
			UpdateVision();
			DrawPath();
		}
	}
}

void HexScene::OnMouseOutHandler(unsigned entity) {

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
	const vec2i mapPosition = maze->ScreenToMapPosition(screenSpace.xy);
	const int index = maze->GetMapIndex(mapPosition);

	if (index < 0) return;

	auto& team = teams[playerTurn];
	auto selected = team.GetSelectedUnit();
	if (!hovered && selected && !path.empty()) {
		selected->path = path;
		moveCount += path.size();
		path.clear();
	} else {
		auto unit = team.GetUnitAt(screenSpace);
		if (!unit) return;
		for (float i = 1.f; i < unit->range; ++i) {
			for (auto& tile : maze->GetTileIndexesAtRange(i, screenSpace.xy)) {
				if (tile < 0) continue;
				auto render = grid[tile];

				auto animation = entities->GetComponent<Animation>(render->entity);
				animation->Animate(
					AnimationBase(false, 0.2f, i * 0.05f, [animation, render]() {
						animation->Animate(
							AnimationBase(false, 0.05f),
							render->tint.a,
							0.3f
						);
					}),
					render->tint.a,
					1.f
				);
			}
		}
	}
}

void HexScene::UnitEnterHandler(unsigned entity) {
	hovered = entities->GetComponent<Transform>(entity);
}

void HexScene::UnitExitHandler(unsigned entity) {
	if (hovered->entity == entity)
		hovered = nullptr;
}

void HexScene::SelectUnitHandler(unsigned entity) {
	teams[playerTurn].SelectUnit(entity);
}

unsigned HexScene::GetCurrentMaxMoves() const {
	return maxMoves * (turnCount / 2 + 1);
}
