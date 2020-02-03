#include "HexScene.h"

// components
#include "Transform.h"
#include "Camera.h"
#include "TeamContainer.h"
// systems
#include "RenderSystem.h"
#include "ButtonSystem.h"
#include "AnimationSystem.h"
#include "ParticleSystem.h"
#include "StateMachine.h"
// states
#include "IdleState.h"
#include "ExploreState.h"
#include "AttackState.h"
// Utils
#include "LoadFNT.h"
#include "InputEvents.h"
// Destination
#include "MenuScene.h"

#include <Events/EventsManager.h>
#include <Math/Math.hpp>
#include <GLFW/glfw3.h>

void HexScene::Awake() {
	Scene::Awake();

	moveDelay = 0.2f;

	Events::EventsManager::GetInstance()->Subscribe("KEY_INPUT", &HexScene::KeyHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("ATTACK", &HexScene::AttackHandler, this);

	gridSize = 12;

	auto font = Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga");

	systems->Subscribe<RenderSystem>(0);
	systems->Subscribe<ButtonSystem>(1);
	systems->Subscribe<AnimationSystem>(2);
	systems->Subscribe<ParticleSystem>(3);
	systems->Subscribe<StateMachine>(4);

	systems->Get<StateMachine>()->AttachState<States::Idle>("IDLE");
	systems->Get<StateMachine>()->AttachState<States::Explore>("EXPLORE");
	systems->Get<StateMachine>()->AttachState<States::Attack>("ATTACK");

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
		debugText->SetActive(true);
		debugText->SetFont(font);
	}

	{
		const unsigned label = entities->Create();
		entities->GetComponent<Transform>(label)->translation.Set(0.f, -9.f, 0.f);
		promptText = entities->AddComponent<Text>(label);
		promptText->SetActive(true);
		promptText->SetFont(font);
		promptText->scale = 0.5f;
	}
	
	maze = new HexMaze(gridSize, PATH);
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

		highlightText = entities->AddComponent<Text>(highlightTile);
		highlightText->SetActive(true);
		highlightText->SetFont(font);
		highlightText->offset.Set(0.f, 1.f, 0.f);
		highlightText->scale = 0.5f;
	}

	teams[0].SetVision(gridSize);
	teams[1].SetVision(gridSize);
}

void HexScene::Update(const float & dt) {
	Scene::Update(dt);

	debugText->text = std::to_string(static_cast<int>(1.f / dt));

	bt += dt;
	if (bt > moveDelay) {
		auto& team = teams[playerTurn];
		if (moveCount < GetCurrentMaxMoves()) {
			const unsigned steps = team.Move(moveDelay, entities);
			if (steps > 0) {
				UpdateVision();
				if (team.IsAI() && team.GetSelectedUnit()) {
					DrawView(team.GetSelectedUnit());
				}
				bt = 0.f;

				moveCount += steps;
			} else {
				if (team.IsAI()) {
					team.MakeMove(entities);
					bt = 0.f;
				}
			}
		} else {
			if (team.IsAI()) {
				entities->GetComponent<StateContainer>(team.GetAI())->queuedState = "IDLE";
				bt = 0.f;
				EndTurn();
			}
		}
	}

	mazeBt += dt;
	if (mazeBt > 0.2f) {
		maze->Update(mazeBt);
			//UpdateVision();
		mazeBt = 0.f;
	}

	promptText->text = std::to_string(GetCurrentMaxMoves() - moveCount) + " moves left.";
}

void HexScene::InitializeGame() {
	bt = 0.f;
	spacePressed = false;

	maze->Generate(vec2i(5, 5), 0.7f);

	maxMoves = 5;
	moveCount = 0;
	turnCount = 0;

	teams[0].SetOpponent(&teams[1]);
	teams[0].SetMaze(maze);
	teams[1].SetOpponent(&teams[0]);
	teams[1].SetMaze(maze);

	if (mode == PVP) {
		teams[0].SetName("Yellow Player");
		teams[0].SetAI(0);
		teams[1].SetName("Cyan Player");
		teams[1].SetAI(0);
	} else if (mode == PVE) {
		teams[0].SetName("Player");
		teams[0].SetAI(0);

		teams[1].SetName("AI");
		teams[1].SetAI(CreateBrain(&teams[1]));
	} else if (mode == AIvAI) {
		teams[0].SetName("Yellow AI");
		teams[0].SetAI(CreateBrain(&teams[0]));

		teams[1].SetName("Cyan AI");
		teams[1].SetAI(CreateBrain(&teams[1]));
	}

	float range = 1.f;
	for (int x = 0; x < 2; ++x) {
		for (int y = 0; y < 2; ++y) {
			teams[0].AddUnit(CreateUnit(x, y, vec4f(1.f, 1.f, 0.f, 1.f), range, 2.f));
			teams[1].AddUnit(CreateUnit(gridSize - 1 - x, gridSize - 1 - y, vec4f(0.f, 1.f, 1.f, 1.f), range, 2.f));
			++range;
		}
	}

	playerTurn = false;
	UpdateVision();
}

void HexScene::SetMode(const unsigned & _mode) {
	mode = _mode;
}

void HexScene::KeyHandler(Events::Event * event) {
	Events::KeyInput* input = static_cast<Events::KeyInput*>(event);

	if (input->key == GLFW_KEY_SPACE && !teams[playerTurn].IsAI()) {
		if (input->action == GLFW_PRESS) {
			spacePressed = true;
			auto selected = teams[playerTurn].GetSelectedUnit();
			if (selected) {
				highlightText->text = "Attack!";
				highlightText->color.Set(1.f);

				UpdateVision();
				DrawView(selected);
			}
		} else if (input->action == GLFW_RELEASE) {
			spacePressed = false;
			UpdateVision();
			DrawPath();
		}
	} else if (input->key == GLFW_KEY_ENTER && input->action == GLFW_RELEASE) {
		EndTurn();
	}

}

void HexScene::UpdateVision() {
	auto& team = teams[playerTurn];
	auto vision = team.GetVision();
	team.UpdateVision();

	const float a = 0.5f;

	for (auto& unit : team.GetUnits()) {
		entities->GetComponent<Render>(unit->transform->entity)->SetActive(true);
	}

	for (auto& unit : teams[!playerTurn].GetUnits()) {
		entities->GetComponent<Render>(unit->transform->entity)->SetActive(false);
	}

	for (unsigned i = 0; i < vision.size(); ++i) {
		if (vision[i]) {
			entities->GetComponent<Animation>(grid[i]->entity)->Animate(
				AnimationBase(false, 0.2f),
				grid[i]->tint,
				maze->GetColour(maze->GetMapData(i))
			);

			if (team.GetOpponentUnit(i)) {
				const unsigned entity = team.GetOpponentUnit(i)->transform->entity; 
				entities->GetComponent<Render>(entity)->SetActive(true);
			}
		} else {
			entities->GetComponent<Animation>(grid[i]->entity)->Animate(
				AnimationBase(false, 0.2f),
				grid[i]->tint,
				vec4f(0.f)
			);
		}
	}
}

void HexScene::Highlight(Render * const tile, const int& tileType) {
	//tile->tint = (tile->tint + vec4f(1.f, 1.f, 0.f, 1.f)) * 0.5f;
	auto color = maze->GetColour(tileType);
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
		const int index = maze->GetMapIndex(pos);
		Highlight(grid[index], maze->GetMapData(index));
	}

	highlightText->text = std::to_string(path.size());
	if (path.size() == GetCurrentMaxMoves() - moveCount)
		highlightText->color.Set(1.f, 0.f, 0.f, 1.f);
	else
		highlightText->color.Set(1.f);
}

void HexScene::DrawView(Unit * const unit) {
	for (auto& index : unit->vision) {
		//grid[index]->tint.a = 1.f;
		Highlight(grid[index], maze->GetMapData(index));
	}
}

void HexScene::DrawAttackArea(const vec3f & position, const float& area) {
	for (float i = 1.f; i < area; ++i) {
		for (auto& tile : maze->GetTileIndexesAtRange(i, position.xy)) {
			if (tile < 0) continue;

			auto render = grid[tile];

			entities->GetComponent<Animation>(render->entity)->Animate(
				AnimationBase(false, 0.2f),
				render->tint,
				Math::Mix(vec4f(1.f, 0.f, 0.f, 1.f), render->tint, 0.5f)
			);
		}
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

Unit * HexScene::CreateUnit(const int & x, const int & y, const vec4f& color, const float& viewRange, const float& AOE) {
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

	auto emitter = entities->AddComponent<ParticleEmitter>(entity);
	emitter->SetActive(true);
	emitter->duration = 0.1f;
	emitter->burstAmount = 30;
	emitter->speed = 5.f;
	emitter->speedRange = 2.f;
	emitter->angleRange.Set(0.f, 0.f, 180.f);
	emitter->lifetime = 0.1f;
	emitter->lifetimeRange = 0.1f;
	emitter->startColor = color;
	emitter->endColor = color;
	emitter->startSize.Set(0.25f);
	emitter->endSize.Set(0.f);

	Unit* unit = new Unit;
	unit->transform = transform;
	unit->viewRange = viewRange;
	unit->AOE = AOE;
	unit->canAttack = true;

	return unit;
}

unsigned HexScene::CreateBrain(Team * const team) {
	unsigned brain = entities->Create();

	auto container = entities->AddComponent<StateContainer>(brain);
	container->SetActive(true);
	container->queuedState = "IDLE";

	auto teamContainer = entities->AddComponent<TeamContainer>(brain);
	teamContainer->team = team;

	return brain;
}

void HexScene::AttackHandler(Events::Event * event) {
	auto attack = static_cast<Events::Attack*>(event);
	Attack(attack->team, attack->position);
}

void HexScene::Attack(Team * const team, const vec3f& position) {
	const vec2i mapPosition = maze->ScreenToMapPosition(position.xy);
	const int index = maze->GetMapIndex(mapPosition);

	auto selected = team->GetSelectedUnit();
	if (selected) {
		if (vfind(selected->vision, index) == selected->vision.end() || !selected->canAttack) return;
		selected->canAttack = false;

		auto& opponent = teams[!playerTurn];
		auto unit = opponent.GetUnitAt(position);
		if (unit && opponent.DestroyUnit(unit)) {
			const unsigned& unitEntity = unit->transform->entity;
			entities->GetComponent<ParticleEmitter>(unitEntity)->Play([this, team, &opponent, unitEntity]() {
				entities->Destroy(unitEntity);
				if (opponent.IsDead()) {
					winner = team->GetName();
					Events::EventsManager::GetInstance()->Trigger("PRESENT_SCENE", new Events::String("MENU"));
				}
			});
		} else if (maze->GetMapData(index) <= 0) {
			maze->SetMapData(index, PATH);
		}

		for (float i = 1.f; i < selected->AOE; ++i) {
			for (auto& tile : maze->GetTilesAtRange(i, position.xy)) {
				auto tileIndex = maze->GetMapIndex(maze->ScreenToMapPosition(tile));
				auto unit = opponent.GetUnitAt(tile);
				if (unit && opponent.DestroyUnit(unit)) {
					const unsigned& unitEntity = unit->transform->entity;
					entities->GetComponent<Render>(unitEntity)->tint.a = 0.f;
					entities->GetComponent<ParticleEmitter>(unitEntity)->Play([this, team, &opponent, unitEntity]() {
						entities->Destroy(unitEntity);
						if (opponent.IsDead()) {
							winner = team->GetName();
							Events::EventsManager::GetInstance()->Trigger("PRESENT_SCENE", new Events::String("MENU"));
						}
					});
				} else if (maze->GetMapData(tileIndex) <= 0) {
					maze->SetMapData(tileIndex, PATH);
				}
			}
		}

		team->Scan(selected, position.xy);
		UpdateVision();
	}
}

void HexScene::OnMouseOverHandler(unsigned entity) {
	auto position = entities->GetComponent<Transform>(entity)->translation;
	highlight->translation = position;

	auto& team = teams[playerTurn];
	auto selected = team.GetSelectedUnit();
	const auto max = GetCurrentMaxMoves();

	if (selected && team.InSight(position) && !team.IsAI()) {
		auto index = maze->GetMapIndex(maze->ScreenToMapPosition(position));

		if (maze->GetMapData(index) == WALL)
			return;

		path = team.GetPath(selected->transform->translation, position);
		const unsigned movesLeft = max - moveCount;
		if (path.size() > movesLeft) {
			path.erase(path.begin() + movesLeft, path.end());
		}

		if (!spacePressed) {
			UpdateVision();
			DrawPath();
		} else {
			UpdateVision();
			DrawView(selected);
			if (selected->canAttack) 
				DrawAttackArea(position, selected->AOE);
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

	if (spacePressed) {
		Attack(&team, screenSpace);
	} else {
		auto selected = team.GetSelectedUnit();

		if (!hovered && selected && !path.empty()) {
			selected->path = path;
			path.clear();
		} else {
			auto unit = team.GetUnitAt(screenSpace);
			if (!unit) return;
			for (float i = 1.f; i < unit->viewRange; ++i) {
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
}

void HexScene::UnitEnterHandler(unsigned entity) {
	hovered = entities->GetComponent<Transform>(entity);
}

void HexScene::UnitExitHandler(unsigned entity) {
	if (hovered->entity == entity)
		hovered = nullptr;
}

void HexScene::SelectUnitHandler(unsigned entity) {
	if (spacePressed) {
		//if (teams[!playerTurn].DestroyUnit(entity)) {
		//	entities->GetComponent<ParticleEmitter>(entity)->Play([this, entity]() {
		//		entities->Destroy(entity);
		//	});
		//	hovered = nullptr;
		//}
	} else {
		teams[playerTurn].SelectUnit(entity);
	}
}

unsigned HexScene::GetCurrentMaxMoves() const {
	return maxMoves + turnCount;
}

void HexScene::EndTurn() {
	highlightText->text = "";
	playerTurn = !playerTurn;
	for (auto& unit : teams[playerTurn].GetUnits()) {
		unit->canAttack = true;
	}

	moveCount = 0;
	++turnCount;
	UpdateVision();
}

void HexScene::PrepareForSegue(Scene * destination) {
	MenuScene* dest = dynamic_cast<MenuScene*>(destination);
	if (dest) {
		for (auto& team : teams) {
			team.DestroyUnits([this](unsigned entity) { 
				entities->Destroy(entity);
			});

			if (team.IsAI()) {
				entities->Destroy(team.GetAI());
			}
		}
		dest->SetTitle(winner + " won!");
	}
}
