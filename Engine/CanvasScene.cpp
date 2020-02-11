#include "CanvasScene.h"

// components
#include "Transform.h"
#include "Render.h"
#include "Text.h"
// systems
#include "ButtonSystem.h"
#include "AnimationSystem.h"
#include "ParticleSystem.h"
// utils
#include "LoadTGA.h"
#include "LoadFNT.h"
#include "InputEvents.h"

#include <Events/EventsManager.h>
#include <Math/Random.hpp>
#include <Math/Math.hpp>
#include <Helpers/FileHelpers.h>
#include <GLFW/glfw3.h>

void CanvasScene::Awake() {

	systems->Subscribe<ButtonSystem>(1);
	systems->Subscribe<AnimationSystem>(2);
	systems->Subscribe<ParticleSystem>(3);

	Scene::Awake();

	graph.BindCreateNodeHandler(&CanvasScene::CreateNode, this);
	graph.BindCreateEdgeHandler(&CanvasScene::CreateEdge, this);
	graph.Create("Files/Data/graph.csv");

	camera = entities->GetComponent<Camera>(mainCamera);
	camera->SetSize(10.f);

	Events::EventsManager::GetInstance()->Subscribe("CURSOR_POSITION_INPUT", &CanvasScene::CursorPositionHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("MOUSE_BUTTON_INPUT", &CanvasScene::MouseButtonHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("DROP_INPUT", &CanvasScene::DropHandler, this);
}

void CanvasScene::Start() {

}

void CanvasScene::Update(const float & dt) {
	Scene::Update(dt);
	graph.DrawEdges();

	if (!mouseDown)
		mouseMoved = false;

	for (auto& e : emitters) {
		e.transform->translation = Math::Lerp(e.transform->translation, e.node->position, dt);

		if (Math::LengthSquared(e.transform->translation - e.node->position) <= 0.5f) {
			const int index = Math::RandMinMax(0, static_cast<int>(e.node->edges.size()));
			const Edge* edge = e.node->edges[index];
			e.node = edge->from == e.node ? edge->to : edge->from;
		}
	}
}

void CanvasScene::FixedUpdate(const float & dt) {
	for (auto& e : emitters) {
		auto info = Helpers::GetFileInfo(e.filepath);
		if (e.lastMod != info.st_mtime) {
			e.lastMod = info.st_mtime;
			UpdateEmitter(e.emitter, e.filepath);
		}
	}
}

void CanvasScene::CursorPositionHandler(Events::Event * event) {
	auto input = static_cast<Events::CursorPositionInput*>(event);
	mouseMoved = true;
	mousePosition = input->position;

	smPosition = camera->ScreenToWorldSpace(input->position);
	smPosition.y = -smPosition.y;

	if (mouseDown && selected) {
		selected->translation = smPosition;
		entityNodeMap[selected->entity]->position = smPosition;
		if (start && end)
			graph.PathFind(start, end);
	}
}

void CanvasScene::MouseButtonHandler(Events::Event * event) {
	auto input = static_cast<Events::MouseButtonInput*>(event);
	if (!mouseOver) {
		if (input->button == GLFW_MOUSE_BUTTON_LEFT && input->action == GLFW_RELEASE) {
			auto position = camera->ScreenToWorldSpace(mousePosition);
			graph.CreateNode(position.x, -position.y, 0.f);
			mouseOver = true;
		}
	}

	if (input->button == GLFW_MOUSE_BUTTON_RIGHT && input->action == GLFW_RELEASE) {
		if (start) {
			if (end) {
				entities->GetComponent<Render>(start->entity)->tint.Set(1.f);
				entities->GetComponent<Render>(end->entity)->tint.Set(1.f);
				start = end = nullptr;
				graph.ClearPath();
			} else {
				end = graph.GetNearestNode(smPosition);
				entities->GetComponent<Render>(end->entity)->tint.Set(1.f, 1.f, 0.f, 1.f);
				graph.PathFind(start, end);
			}
		} else {
			start = graph.GetNearestNode(smPosition);
			entities->GetComponent<Render>(start->entity)->tint.Set(1.f, 1.f, 0.f, 1.f);
		}
	}
}

void CanvasScene::DropHandler(Events::Event * event) {
	auto drop = static_cast<Events::DropInput*>(event);
	
	for (int i = 0; i < drop->count; ++i) {
		auto filepath = drop->paths[i];

		auto transform = CreateEmitter(filepath);
		transform->translation = smPosition;
	}
}

void CanvasScene::OnMouseOverHandler(unsigned entity) {
	entities->GetComponent<Animation>(entity)->Animate(
		AnimationBase(false, 0.2f),
		entities->GetComponent<Transform>(entity)->scale,
		vec3f(1.25f)
	);
	mouseOver = true;
}

void CanvasScene::OnMouseOutHandler(unsigned entity) {
	entities->GetComponent<Animation>(entity)->Animate(
		AnimationBase(false, 0.2f),
		entities->GetComponent<Transform>(entity)->scale,
		vec3f(1.f)
	);
	mouseOver = false;
}

void CanvasScene::OnMouseDownHandler(unsigned entity) {
	entities->GetComponent<Animation>(entity)->Animate(
		AnimationBase(false, 0.2f),
		entities->GetComponent<Transform>(entity)->scale,
		vec3f(1.f)
	);

	mouseDown = true;

	if (selected) {
		graph.CreateEdge(entityNodeMap[selected->entity]->id, entityNodeMap[entity]->id);
		selected = nullptr;
	} else {
		selected = entities->GetComponent<Transform>(entity);
	}
}

void CanvasScene::OnMouseUpHandler(unsigned entity) {
	entities->GetComponent<Animation>(entity)->Animate(
		AnimationBase(false, 0.2f),
		entities->GetComponent<Transform>(entity)->scale,
		vec3f(1.f)
	);

	mouseDown = false;
}

void CanvasScene::OnClick(unsigned entity) {
	if (mouseMoved) {
		selected = nullptr;
	} else {
		if (selected) {
		}
	}
}

void CanvasScene::CreateNode(Node * node) {
	const auto entity = entities->Create();

	entityNodeMap[entity] = node;
	node->entity = entity;

	entities->GetComponent<Transform>(entity)->translation = node->position;

	auto render = entities->AddComponent<Render>(entity);
	render->SetActive(true);
	render->SetTexture("Files/Textures/circle.tga");

	const auto text = entities->AddComponent<Text>(entity);
	text->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
	text->SetActive(true);
	text->scale = 0.5f;
	text->text = std::to_string(node->id);
	text->color.Set(0.f, 0.f, 0.f, 1.f);

	auto button = entities->AddComponent<Button>(entity);
	button->SetActive(true);
	button->BindHandler(MOUSE_OVER, &CanvasScene::OnMouseOverHandler, this);
	button->BindHandler(MOUSE_OUT, &CanvasScene::OnMouseOutHandler, this);
	button->BindHandler(MOUSE_DOWN, &CanvasScene::OnMouseDownHandler, this);
	button->BindHandler(MOUSE_UP, &CanvasScene::OnMouseUpHandler, this);
	button->BindHandler(MOUSE_CLICK, &CanvasScene::OnClick, this);

	auto animation = entities->AddComponent<Animation>(entity);
	animation->SetActive(true);
}

void CanvasScene::CreateEdge(Edge * edge) {
	if (start && end) {
		graph.PathFind(start, end);
	}
}

Transform* CanvasScene::CreateEmitter(const std::string& filepath) {
	const unsigned entity = entities->Create();

	auto transform = entities->GetComponent<Transform>(entity);

	const auto emitter = entities->AddComponent<ParticleEmitter>(entity);
	emitter->SetActive(true);
	UpdateEmitter(emitter, filepath);

	emitters.push_back({
		transform,
		emitter,
		graph.GetNearestNode(transform->translation),
		filepath,
		Helpers::GetFileInfo(filepath).st_mtime
	});

	return transform;
}

void CanvasScene::UpdateEmitter(ParticleEmitter* emitter, const std::string& filepath) {
	LuaScript* script = new LuaScript(filepath);

	emitter->age = script->Get<float>("emitter.age");
	emitter->duration = script->Get<float>("emitter.duration");

	emitter->spawnInterval = script->Get<float>("emitter.spawnInterval");
	emitter->spawnTimer = script->Get<float>("emitter.spawnTimer");
	emitter->burstAmount = script->Get<int>("emitter.burstAmount");

	emitter->loop = script->Get<bool>("emitter.loop");
	emitter->lifetime = script->Get<float>("emitter.lifetime");
	emitter->lifetimeRange = script->Get<float>("emitter.lifetimeRange");

	auto angle = script->GetVector<float>("emitter.angle");
	emitter->angle.Set(angle[0], angle[1], angle[2]);
	auto angleRange = script->GetVector<float>("emitter.angleRange");
	emitter->angleRange.Set(angleRange[0], angleRange[1], angleRange[2]);

	emitter->speed = script->Get<float>("emitter.speed");
	emitter->speedRange = script->Get<float>("emitter.speedRange");

	auto offset = script->GetVector<float>("emitter.offset");
	emitter->offset.Set(offset[0], offset[1], offset[2]);
	auto positionRange = script->GetVector<float>("emitter.positionRange");
	emitter->positionRange.Set(positionRange[0], positionRange[1], positionRange[2]);

	auto gravity = script->GetVector<float>("emitter.gravity");
	emitter->gravity.Set(gravity[0], gravity[1], gravity[2]);

	emitter->accelRad = script->Get<float>("emitter.accelRad");
	emitter->accelRadRange = script->Get<float>("emitter.accelRadRange");

	auto startSize = script->GetVector<float>("emitter.startSize");
	emitter->startSize.Set(startSize[0], startSize[1], startSize[2]);
	auto startSizeRange = script->GetVector<float>("emitter.startSizeRange");
	emitter->startSizeRange.Set(startSizeRange[0], startSizeRange[1], startSizeRange[2]);

	auto endSize = script->GetVector<float>("emitter.endSize");
	emitter->endSize.Set(endSize[0], endSize[1], endSize[2]);
	auto endSizeRange = script->GetVector<float>("emitter.endSizeRange");
	emitter->endSizeRange.Set(endSizeRange[0], endSizeRange[1], endSizeRange[2]);

	emitter->texture = Load::TGA(script->Get<std::string>("emitter.texture"));

	auto startColor = script->GetVector<float>("emitter.startColor");
	emitter->startColor.Set(startColor[0], startColor[1], startColor[2], startColor[3]);
	auto startColorRange = script->GetVector<float>("emitter.startColorRange");
	emitter->startColorRange.Set(startColorRange[0], startColorRange[1], startColorRange[2], startColorRange[3]);

	auto endColor = script->GetVector<float>("emitter.endColor");
	emitter->endColor.Set(endColor[0], endColor[1], endColor[2], endColor[3]);
	auto endColorRange = script->GetVector<float>("emitter.endColorRange");
	emitter->endColorRange.Set(endColorRange[0], endColorRange[1], endColorRange[2], endColorRange[3]);

	delete script;

}
