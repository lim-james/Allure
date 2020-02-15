#include "GameScene.h"

// systems
#include "RenderSystem.h"
#include "ButtonSystem.h"
#include "AnimationSystem.h"
#include "StateMachine.h"
#include "State.h"

#include "LoadFNT.h"
#include "InputEvents.h"

#include <Math/Math.hpp>
#include <Math/Random.hpp>
#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void GameScene::Awake()
{
	systems->Subscribe<ButtonSystem>(1);
	systems->Subscribe<AnimationSystem>(2);

	Scene::Awake();

	selected = nullptr;
	start = end = nullptr;
	
	selection = false;

	script = new LuaScript("Files/Scripts/Graph.lua");

	Events::EventsManager::GetInstance()->Subscribe("KEY_INPUT", &GameScene::KeyHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("TEXT_INPUT", &GameScene::TextHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("CURSOR_POSITION_INPUT", &GameScene::CursorPositionHandler, this);
	//Events::EventsManager::GetInstance()->Subscribe("MOUSE_BUTTON_INPUT", &GameScene::MouseButtonHandler, this);

	camera = entities->GetComponent<Camera>(mainCamera);
	camera->SetActive(true);
	camera->clearColor.Set(0.f);	
	entities->GetComponent<Camera>(mainCamera)->SetSize(10.f);

	graph.BindCreateNodeHandler(&GameScene::CreateNode, this);
	graph.BindCreateEdgeHandler(&GameScene::CreateEdge, this);
	//graph.Create("Files/Data/graph.csv");
	graph.Create(script);

	auto courierNew = Load::FNT("Files/Fonts/CourierNew.fnt", "Files/Fonts/CourierNew.tga");

	{
		// text field
		textField = entities->Create();

		auto transform = entities->GetComponent<Transform>(textField);
		//transform->translation.Set(0.0f, -5.f, -10.f);
		transform->translation.Set(-10.0f, 9.f, 0.f);
		transform->scale.x = 15.0f;
		transform->scale.z = 0.f;

		auto render = entities->AddComponent<Render>(textField);
		render->SetActive(true);
		render->tint.Set(0.2f, 0.2f, 0.2f, 0.0f);

		auto text = entities->AddComponent<Text>(textField);
		text->SetActive(true);
		text->SetFont(courierNew);
		text->text = "[Tab] to enter console";
		text->scale = 0.5f;
		text->paragraphAlignment = PARAGRAPH_LEFT;
		text->offset.x = 0.2f;

		auto button = entities->AddComponent<Button>(textField);
		button->SetActive(true);
		button->BindHandler(MOUSE_CLICK, &GameScene::TextFieldDidSelect, this);
	}

	{
		cursor = entities->Create();

		auto transform = entities->GetComponent<Transform>(cursor);
		transform->scale.Set(0.1f, 0.7f, 1.0f);

		auto render = entities->AddComponent<Render>(cursor);
		render->SetActive(true);
		render->tint.Set(1.0f);
	}

	player.id = entities->Create();
	player.transform = entities->GetComponent<Transform>(player.id);
	player.transform->scale.Set(2, 2, 1);
	player.node = graph.GetNearestNode(player.transform->translation);
	player.distance = 5.f;
	player.speed = 1.f;
	auto render = entities->AddComponent<Render>(player.id);
	render->SetActive(true);
	render->SetTexture("Files/Textures/player.tga");
}

void GameScene::Update(const float & dt)
{
	Scene::Update(dt);
	graph.DrawEdges();

	if (!mouseDown)
		mouseMoved = false;

	bt += dt;
	if (bt > 0.2f) {
		graph.Explore();
		bt = 0.f;
	}

	if (Math::LengthSquared(player.transform->translation - vec3f(smPosition, 0)) <= player.distance) {
		player.transform->translation = Math::Lerp(player.transform->translation, vec3f(smPosition, 0), dt);
		returnClosest = false;
	}
	else {
		if (!returnClosest) {
			player.node = graph.GetNearestNode(player.transform->translation);
			returnClosest = true;
		}

		player.transform->translation = Math::Lerp(player.transform->translation, player.node->position, dt);

		if (Math::LengthSquared(player.transform->translation - player.node->position) <= 0.5f) {
			const int index = Math::RandMinMax(0, static_cast<int>(player.node->edges.size()));
			const Edge* edge = player.node->edges[index];
			player.node = edge->from == player.node ? edge->to : edge->from;
		}
	}
}

void GameScene::FixedUpdate(const float& dt) {
	UpdateNodePosition();
}

void GameScene::TextFieldDidSelect(unsigned target) {
	selection = true;
	cursorPos = entities->GetComponent<Text>(textField)->text.length();
	UpdateCursorOffset(target);
}

void GameScene::UpdateCursorOffset(unsigned target) {
	auto transform = entities->GetComponent<Transform>(target);
	auto text = entities->GetComponent<Text>(target);
	auto font = text->GetFont();

	const float scale = text->scale;

	std::vector<float> lineOffset;
	float numLines = 0;
	vec2f size(0.f);

	const auto& content = text->text;

	for (unsigned i = 0; i <= content.size(); ++i) {
		auto& c = content[i];

		switch (c) {
		case '\0':
		case '\n':
			switch (text->paragraphAlignment) {
			case PARAGRAPH_CENTER:
				lineOffset.push_back(size.x * scale * 0.5f);
				break;
			case PARAGRAPH_RIGHT:
				lineOffset.push_back(transform->scale.x * -0.5f - size.x * -scale);
				break;
			default:
				lineOffset.push_back(transform->scale.x * 0.5f);
				break;
			}
			size.x = 0.f;
			++numLines;
			break;
		default:
			size.x += font->characters[c].xAdvance;
			break;
		}
	}

	size.y = numLines + 1;
	size.y *= font->lineHeight * text->lineSpacing * text->scale;

	transform->scale.y = size.y;

	size.y = numLines - 1;
	size.y *= font->lineHeight * text->lineSpacing * text->scale;

	const vec3f translation = transform->GetWorldTranslation() + text->offset;
	vec3f position(0.f);
	position.x = translation.x - lineOffset[0];

	switch (text->verticalAlignment) {
	case ALIGN_MIDDLE:
		position.y = translation.y + size.y * 0.5f;
		break;
	case ALIGN_BOTTOM:
		position.y = translation.y - transform->scale.y * 0.5f + size.y;
		break;
	default:
		position.y = translation.y + transform->scale.y * 0.5f;
		break;
	}

	int lineNumer = 0;
	for (int i = 0; i < cursorPos; ++i) {
		const char& c = text->text[i];
		if (c == '\0') continue;

		switch (c) {
		case '\n':
			position.y -= font->lineHeight * text->lineSpacing * text->scale;
			position.x = translation.x - lineOffset[++lineNumer];
			break;
		default:
			const auto& character = font->characters[c];
			const vec3f offset = character.rect.origin * scale;

			position.x += character.xAdvance * text->characterSpacing * scale;
			break;
		}
	}

	position.z = -10.f;
	entities->GetComponent<Transform>(cursor)->translation = position;
}

void GameScene::CreateNode(Node * node)
{
	const auto entity = entities->Create();

	entityNodeMap[entity] = node;
	node->entity = entity;

	entities->GetComponent<Transform>(entity)->translation = node->position;
	//entities->GetComponent<Transform>(entity)->scale.Set(1.f);

	auto render = entities->AddComponent<Render>(entity);
	render->SetActive(true);
	render->SetTexture("Files/Textures/hexagon.tga");

	//const auto text = entities->AddComponent<Text>(entity);
	//text->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
	//text->SetActive(true);
	//text->scale = 0.5f;
	//text->text = std::to_string(node->id);
	//text->color.Set(0.f, 0.f, 0.f, 1.f);

	//auto button = entities->AddComponent<Button>(entity);
	//button->SetActive(true);
	//button->BindHandler(MOUSE_OVER, &GameScene::OnMouseOverHandler, this);
	//button->BindHandler(MOUSE_OUT, &GameScene::OnMouseOutHandler, this);
	//button->BindHandler(MOUSE_DOWN, &GameScene::OnMouseDownHandler, this);
	//button->BindHandler(MOUSE_UP, &GameScene::OnMouseUpHandler, this);
	//button->BindHandler(MOUSE_CLICK, &GameScene::OnClick, this);

	auto animation = entities->AddComponent<Animation>(entity);
	animation->SetActive(true);
}

void GameScene::CreateEdge(Edge * edge)
{
	if (start && end) {
		graph.PathFind(start, end);
	}
}

void GameScene::TextHandler(Events::Event* event) {
	auto input = static_cast<Events::TextInput*>(event);

	if (selection) {
		const auto c = static_cast<Events::TextInput*>(event)->data;
		auto& text = entities->GetComponent<Text>(textField)->text;
		text.insert(text.begin() + cursorPos, c);
		++cursorPos;
		UpdateCursorOffset(textField);
		//DidChange(this);
	}
}

void GameScene::KeyHandler(Events::Event* event) {
	auto input = static_cast<Events::KeyInput*>(event);

	if (input->key == GLFW_KEY_TAB && input->action == !GLFW_RELEASE)
	{
		//UpdateNodePosition();
		entities->GetComponent<Text>(textField)->text = "here";
		//script->Set<int>("graph.node1.posx", 10, true, true);
		//selection = true;
	}

	if (input->action == GLFW_PRESS && input->key == GLFW_KEY_GRAVE_ACCENT) {

	}
	else if (input->key == GLFW_KEY_BACKSPACE && input->action != GLFW_RELEASE) {
		auto& text = entities->GetComponent<Text>(textField)->text;
		if (text.length() && cursorPos > 0) {
			text.erase(text.begin() + cursorPos - 1);
			--cursorPos;
			UpdateCursorOffset(textField);
		}
	}
	else if (input->key == GLFW_KEY_ENTER && input->action == GLFW_PRESS) {
		auto& text = entities->GetComponent<Text>(textField)->text;
		//if (isCaps) {
		//	text.insert(text.begin() + cursorPos, '\n');
		//	++cursorPos;
		//	UpdateCursorOffset(textField);
		//}
		//else {
		//	//DidReturn(this);
		//	text.clear();
		//	cursorPos = 0;
		//	UpdateCursorOffset(textField);
		//}
		if (text == "player.distance=15") {
			player.distance = 15;
		}
		if (text == "player.alpha=0.5") {
			player.alpha = 0.5f;
			entities->GetComponent<Render>(player.id);
		}

		text = "";
	}
	else if (input->key == GLFW_KEY_LEFT && input->action != GLFW_RELEASE) {
		if (cursorPos != 0) {
			--cursorPos;
			UpdateCursorOffset(textField);
		}
	}
	else if (input->key == GLFW_KEY_RIGHT && input->action != GLFW_RELEASE) {
		auto text = entities->GetComponent<Text>(textField);
		if (cursorPos != text->text.length()) {
			++cursorPos;
			UpdateCursorOffset(textField);
		}
	}
	else if (input->key == GLFW_KEY_LEFT_SHIFT) {
		if (input->action == GLFW_PRESS) {
			isCaps = true;
		}
		else if (input->action == GLFW_RELEASE) {
			isCaps = false;
		}
	}
}

void GameScene::CursorPositionHandler(Events::Event * event)
{
	Events::CursorPositionInput* input = static_cast<Events::CursorPositionInput*>(event);
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

void GameScene::MouseButtonHandler(Events::Event * event)
{
	Events::MouseButtonInput* input = static_cast<Events::MouseButtonInput*>(event);
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
			}
			else {
				end = graph.GetNearestNode(smPosition);
				entities->GetComponent<Render>(end->entity)->tint.Set(1.f, 1.f, 0.f, 1.f);
				graph.PathFind(start, end);
			}
		}
		else {
			start = graph.GetNearestNode(smPosition);
			entities->GetComponent<Render>(start->entity)->tint.Set(1.f, 1.f, 0.f, 1.f);
		}
	}
}

void GameScene::OnMouseOverHandler(unsigned entity)
{
	entities->GetComponent<Animation>(entity)->Animate(
		AnimationBase(false, 0.2f),
		entities->GetComponent<Transform>(entity)->scale,
		vec3f(1.25f)
	);
	mouseOver = true;
}

void GameScene::OnMouseOutHandler(unsigned entity)
{
	entities->GetComponent<Animation>(entity)->Animate(
		AnimationBase(false, 0.2f),
		entities->GetComponent<Transform>(entity)->scale,
		vec3f(1.f)
	);
	mouseOver = false;
}

void GameScene::OnMouseDownHandler(unsigned entity)
{
	entities->GetComponent<Animation>(entity)->Animate(
		AnimationBase(false, 0.2f),
		entities->GetComponent<Transform>(entity)->scale,
		vec3f(1.f)
	);

	mouseDown = true;

	if (selected) {
		graph.CreateEdge(entityNodeMap[selected->entity]->id, entityNodeMap[entity]->id);
		selected = nullptr;
	}
	else {
		selected = entities->GetComponent<Transform>(entity);
	}
}

void GameScene::OnMouseUpHandler(unsigned entity)
{
	entities->GetComponent<Animation>(entity)->Animate(
		AnimationBase(false, 0.2f),
		entities->GetComponent<Transform>(entity)->scale,
		vec3f(1.f)
	);

	mouseDown = false;
}

void GameScene::OnClick(unsigned entity)
{
	if (mouseMoved) {
		selected = nullptr;
	}
	else {
		if (selected) {
		}
	}
}

void GameScene::UpdateNodePosition() {
	LuaScript* graphscript = new LuaScript("Files/Scripts/GraphUpdate.lua");
	int i = 1;
	for (auto nodes : graph.nodes) {
		std::string tempx = "graph.node";
		tempx += std::to_string(i);
		tempx += ".posx";
		std::string tempy = "graph.node";
		tempy += std::to_string(i);
		tempy += ".posy";

		int posX = graphscript->Get<int>(tempx);
		int posY = graphscript->Get<int>(tempy);

		nodes->position.Set(posX, posY, 0);
		auto render = entities->GetComponent<Render>(nodes->entity);
		++i;
	}
	delete graphscript;
}