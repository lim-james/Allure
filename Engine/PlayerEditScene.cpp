#include "PlayerEditScene.h"

// systems
#include "RenderSystem.h"
#include "ButtonSystem.h"
#include "AnimationSystem.h"

#include "LoadFNT.h"
#include "InputEvents.h"

#include <Math/Random.hpp>
#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>


void PlayerEditScene::Awake()
{
	systems->Subscribe<ButtonSystem>(1);
	systems->Subscribe<AnimationSystem>(2);

	Scene::Awake();

	selection = false;

	Events::EventsManager::GetInstance()->Subscribe("KEY_INPUT", &PlayerEditScene::KeyHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("TEXT_INPUT", &PlayerEditScene::TextHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("CURSOR_POSITION_INPUT", &PlayerEditScene::CursorPositionHandler, this);
	//Events::EventsManager::GetInstance()->Subscribe("CURSOR_POSITION_INPUT", &PlayerEditScene::CursorPositionHandler, this);
	//Events::EventsManager::GetInstance()->Subscribe("MOUSE_BUTTON_INPUT", &PlayerEditScene::MouseButtonHandler, this);
	//Events::EventsManager::GetInstance()->Subscribe("KEY_INPUT", &PlayerEditScene::KeyDownHander, this);

	camera = entities->GetComponent<Camera>(mainCamera);
	camera->SetActive(true);
	camera->clearColor.Set(0.f);
	entities->GetComponent<Camera>(mainCamera)->SetSize(10.f);

	player.id = entities->Create();
	entities->GetComponent<Transform>(player.id)->translation.Set(-10, 0, 0);
	player.transform = entities->GetComponent<Transform>(player.id);
	player.transform->scale.Set(15, 15, 1);
	auto render = entities->AddComponent<Render>(player.id);
	render->SetActive(true);
	render->SetTexture("Files/Textures/player.tga");
	player.speed = 1.f;
	player.distance = 5.f;

	auto courierNew = Load::FNT("Files/Fonts/CourierNew.fnt", "Files/Fonts/CourierNew.tga");

	{
		// text field
		speedText = entities->Create();

		auto transform = entities->GetComponent<Transform>(speedText);
		//transform->translation.Set(0.0f, -5.f, -10.f);
		transform->translation.Set(10.0f, 7.f, 0.f);
		transform->scale.x = 15.0f;
		transform->scale.z = 0.f;

		auto render = entities->AddComponent<Render>(speedText);
		render->SetActive(true);
		render->tint.Set(0.2f, 0.2f, 0.2f, 0.0f);

		auto text = entities->AddComponent<Text>(speedText);
		text->SetActive(true);
		text->SetFont(courierNew);
		text->text = "Speed: " + std::to_string(player.speed);
		text->scale = 0.5f;
		text->paragraphAlignment = PARAGRAPH_LEFT;
		text->offset.x = 0.2f;

		auto button = entities->AddComponent<Button>(speedText);
		button->SetActive(true);
		button->BindHandler(MOUSE_CLICK, &PlayerEditScene::TextFieldDidSelect, this);
	}

	{
		// text field
		distanceText = entities->Create();

		auto transform = entities->GetComponent<Transform>(distanceText);
		//transform->translation.Set(0.0f, -5.f, -10.f);
		transform->translation.Set(10.0f, 6.f, 0.f);
		transform->scale.x = 15.0f;
		transform->scale.z = 0.f;

		auto render = entities->AddComponent<Render>(distanceText);
		render->SetActive(true);
		render->tint.Set(0.2f, 0.2f, 0.2f, 0.0f);

		auto text = entities->AddComponent<Text>(distanceText);
		text->SetActive(true);
		text->SetFont(courierNew);
		text->text = "Distance: " + std::to_string(player.distance);
		text->scale = 0.5f;
		text->paragraphAlignment = PARAGRAPH_LEFT;
		text->offset.x = 0.2f;

		auto button = entities->AddComponent<Button>(distanceText);
		button->SetActive(true);
		button->BindHandler(MOUSE_CLICK, &PlayerEditScene::TextFieldDidSelect, this);
	}

	{
		// text field
		distanceText = entities->Create();

		auto transform = entities->GetComponent<Transform>(distanceText);
		//transform->translation.Set(0.0f, -5.f, -10.f);
		transform->translation.Set(10.0f, 6.f, 0.f);
		transform->scale.x = 15.0f;
		transform->scale.z = 0.f;

		auto render = entities->AddComponent<Render>(distanceText);
		render->SetActive(true);
		render->tint.Set(0.2f, 0.2f, 0.2f, 0.0f);

		auto text = entities->AddComponent<Text>(distanceText);
		text->SetActive(true);
		text->SetFont(courierNew);
		text->text = "Distance: " + std::to_string(player.distance);
		text->scale = 0.5f;
		text->paragraphAlignment = PARAGRAPH_LEFT;
		text->offset.x = 0.2f;

		auto button = entities->AddComponent<Button>(distanceText);
		button->SetActive(true);
		button->BindHandler(MOUSE_CLICK, &PlayerEditScene::ProceedToGameScene, this);
	}

	{
		// text field
		const unsigned label = entities->Create();

		auto transform = entities->GetComponent<Transform>(label);
		//transform->translation.Set(0.0f, -5.f, -10.f);
		transform->translation.Set(10.0f, -6.f, 0.f);
		transform->scale.x = 5.0f;
		transform->scale.z = 0.f;

		auto render = entities->AddComponent<Render>(label);
		render->SetActive(true);
		render->SetTexture("Files/Textures/circle.tga");
		render->tint.Set(0.2f, 0.2f, 0.2f, 1.0f);

		auto text = entities->AddComponent<Text>(label);
		text->SetActive(true);
		text->SetFont(courierNew);
		text->text = "Play";
		text->scale = 0.5f;
		text->paragraphAlignment = PARAGRAPH_LEFT;
		text->offset.x = 0.2f;

		auto button = entities->AddComponent<Button>(label);
		button->SetActive(true);
		button->BindHandler(MOUSE_CLICK, &PlayerEditScene::ProceedToGameScene, this);
	}

	//{
	//	const unsigned label = entities->Create();
	//	entities->GetComponent<Transform>(label)->scale.Set(7.f, 1.5f, 0.f);
	//	const auto button = entities->AddComponent<Button>(label);
	//	const auto text = entities->AddComponent<Text>(label);
	//	text->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
	//	text->SetActive(true);
	//	text->scale = 0.5f;
	//	text->text = "Button";
	//	button->BindHandler(MOUSE_DOWN, &PlayerEditScene::ButtonFunction, this);
	//}

	//{
	//	const unsigned label = entities->Create();
	//	entities->GetComponent<Transform>(label)->translation.Set(0.f, -2.f, 0.f);
	//	const auto render = entities->AddComponent<Render>(label);
	//	render->SetActive(true);
	//	render->tint.Set(1.f, 1.f, 1.f, 0.f);
	//	//render->SetCellRect(0.f, 0.f, 5.f, 2.f);
	//	const auto button = entities->AddComponent<Button>(label);
	//	const auto text = entities->AddComponent<Text>(label);
	//	text->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
	//	text->SetActive(true);
	//	text->scale = 0.5f;
	//	text->text = "Button";
	//	button->BindHandler(MOUSE_DOWN, &PlayerEditScene::ButtonFunction, this);
	//}


	//script = new LuaScript("Files/Scripts/States.lua");
}

void PlayerEditScene::ProceedToGameScene(unsigned target) {
	Events::EventsManager::GetInstance()->Trigger("PRESENT_SCENE", new Events::String("GAME"));
}

void PlayerEditScene::TextFieldDidSelect(unsigned target) {
	selection = true;
	selected = target;
	cursorPos = entities->GetComponent<Text>(selected)->text.length();
	UpdateCursorOffset(target);
}

void PlayerEditScene::TextHandler(Events::Event* event) {
	auto input = static_cast<Events::TextInput*>(event);

	if (selection) {
		const auto c = static_cast<Events::TextInput*>(event)->data;
		auto& text = entities->GetComponent<Text>(selected)->text;
		text.insert(text.begin() + cursorPos, c);
		++cursorPos;
		UpdateCursorOffset(selected);
		//DidChange(this);
	}
}

void PlayerEditScene::UpdateCursorOffset(unsigned target) {
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

void PlayerEditScene::Update(const float & dt)
{
	Scene::Update(dt);
}

void PlayerEditScene::CursorPositionHandler(Events::Event * event)
{
	Events::CursorPositionInput* input = static_cast<Events::CursorPositionInput*>(event);
	mouseMoved = true;
	mousePosition = input->position;

	smPosition = camera->ScreenToWorldSpace(input->position);
	smPosition.y = -smPosition.y;

	//if (mouseDown && selected) {
	//	selected->translation = smPosition;
	//	entityNodeMap[selected->entity]->position = smPosition;
	//	if (start && end)
	//		graph.PathFind(start, end);
	//}
}

void PlayerEditScene::KeyHandler(Events::Event* event) {
	auto input = static_cast<Events::KeyInput*>(event);

	if (input->key == GLFW_KEY_TAB && input->action == !GLFW_RELEASE)
	{
		selection = true;
		selected = speedText;
		//UpdateNodePosition();
		//script->Set<int>("graph.node1.posx", 10, true, true);
		//selection = true;
	}

	if (input->action == GLFW_PRESS && input->key == GLFW_KEY_GRAVE_ACCENT) {

	}
	else if (input->key == GLFW_KEY_BACKSPACE && input->action != GLFW_RELEASE) {
		auto& text = entities->GetComponent<Text>(selected)->text;
		if (text.length() && cursorPos > 0) {
			text.erase(text.begin() + cursorPos - 1);
			--cursorPos;
			UpdateCursorOffset(selected);
		}
	}
	else if (input->key == GLFW_KEY_ENTER && input->action == GLFW_PRESS) {
		auto& text = entities->GetComponent<Text>(selected)->text;
		if (isCaps) {
			text.insert(text.begin() + cursorPos, '\n');
			++cursorPos;
			UpdateCursorOffset(selected);
		}
		else {
			//DidReturn(this);
			text.clear();
			cursorPos = 0;
			UpdateCursorOffset(selected);
		}
	}
	else if (input->key == GLFW_KEY_LEFT && input->action != GLFW_RELEASE) {
		if (cursorPos != 0) {
			--cursorPos;
			UpdateCursorOffset(selected);
		}
	}
	else if (input->key == GLFW_KEY_RIGHT && input->action != GLFW_RELEASE) {
		auto text = entities->GetComponent<Text>(selected);
		if (cursorPos != text->text.length()) {
			++cursorPos;
			UpdateCursorOffset(selected);
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