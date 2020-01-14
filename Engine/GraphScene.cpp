#include "GraphScene.h"

// systems
#include "RenderSystem.h"
#include "ButtonSystem.h"
#include "AnimationSystem.h"

#include "LoadFNT.h"
#include "InputEvents.h"

#include <Math/Random.hpp>
#include <Events/EventsManager.h>

void GraphScene::Awake() {
	Scene::Awake();

	selected = nullptr;

	Events::EventsManager::GetInstance()->Subscribe("CURSOR_POSITION_INPUT", &GraphScene::CursorPositionHandler, this);

	systems->Subscribe<RenderSystem>(0);
	systems->Subscribe<ButtonSystem>(1);
	systems->Subscribe<AnimationSystem>(2);

	graph.BindCreateNodeHandler(&GraphScene::CreateNode, this);
	graph.BindCreateEdgeHandler(&GraphScene::CreateEdge, this);
	graph.Create("Files/Data/graph.csv");

	const unsigned cam = entities->Create();
	entities->GetComponent<Transform>(cam)->translation.z = 1.f;
	camera = entities->AddComponent<Camera>(cam);
	camera->SetSize(10);
	camera->SetActive(true);
	camera->clearColor.Set(0.f);

	const unsigned label = entities->Create();
	entities->GetComponent<Transform>(label)->translation.Set(0.f, -9.f, 0.f);
	const auto text = entities->AddComponent<Text>(label);
	text->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
	text->SetActive(true);
	text->scale = 0.5f;
	text->text = "[Enter] to toggle lines.";
}

void GraphScene::Update(const float & dt) {
	Scene::Update(dt);
	graph.DrawEdges();

	if (!mouseDown)
		mouseMoved = false;
}

void GraphScene::CreateNode(Node * node) {
	const auto entity = entities->Create();

	entityNodeMap[entity] = node;

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
	button->BindHandler(MOUSE_OVER, &GraphScene::OnMouseOverHandler, this);
	button->BindHandler(MOUSE_OUT, &GraphScene::OnMouseOutHandler, this);
	button->BindHandler(MOUSE_DOWN, &GraphScene::OnMouseDownHandler, this);
	button->BindHandler(MOUSE_UP, &GraphScene::OnMouseUpHandler, this);
	button->BindHandler(MOUSE_CLICK, &GraphScene::OnClick, this);

	auto animation = entities->AddComponent<Animation>(entity);
	animation->SetActive(true);
}

void GraphScene::CreateEdge(Edge * edge) {
	const auto entity = entities->Create();

	const auto position = (edge->from->position + edge->to->position) * 0.5f;
	entities->GetComponent<Transform>(entity)->translation = position;

	const auto text = entities->AddComponent<Text>(entity);
	text->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
	text->SetActive(true);
	text->scale = 0.5f;
	text->text = std::to_string(edge->weight);
	text->color.Set(1.f);
}

void GraphScene::CursorPositionHandler(Events::Event * event) {
	Events::CursorPositionInput* input = static_cast<Events::CursorPositionInput*>(event);
	mouseMoved = true;

	if (mouseDown && selected) {
		auto position = camera->ScreenToWorldSpace(input->position);
		position.y = -position.y;
		selected->translation = position;
		entityNodeMap[selected->entity]->position = position;
	}
}


void GraphScene::OnMouseOverHandler(unsigned entity) {
	entities->GetComponent<Animation>(entity)->Animate(
		AnimationBase(false, 0.2f),
		entities->GetComponent<Transform>(entity)->scale,
		vec3f(0.7f)
	);
}

void GraphScene::OnMouseOutHandler(unsigned entity) {
	entities->GetComponent<Animation>(entity)->Animate(
		AnimationBase(false, 0.2f),
		entities->GetComponent<Transform>(entity)->scale,
		vec3f(1.f)
	);
}

void GraphScene::OnMouseDownHandler(unsigned entity) {
	entities->GetComponent<Animation>(entity)->Animate(
		AnimationBase(false, 0.2f),
		entities->GetComponent<Transform>(entity)->scale,
		vec3f(1.2f)
	);
	
	mouseDown = true;

	if (selected) {
		graph.CreateEdge(entityNodeMap[selected->entity]->id, entityNodeMap[entity]->id);
		selected = nullptr;
	} else {
		selected = entities->GetComponent<Transform>(entity);
	}
}

void GraphScene::OnMouseUpHandler(unsigned entity) {
	entities->GetComponent<Animation>(entity)->Animate(
		AnimationBase(false, 0.2f),
		entities->GetComponent<Transform>(entity)->scale,
		vec3f(1.f)
	);

	mouseDown = false;
}

void GraphScene::OnClick(unsigned entity) {
	if (mouseMoved) {
		selected = nullptr;
	} else {
		if (selected) {
		}
	}
}
