#include "TitleScene.h"

// components
#include "Render.h"
// Systems
#include "ButtonSystem.h"
// Utils
#include "LoadFNT.h"
#include "InputEvents.h"

#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void TitleScene::Awake() {
	systems->Subscribe<ButtonSystem>(1);

	Scene::Awake();

	et = 0.0f;

	entities->GetComponent<Camera>(mainCamera)->SetSize(10.f);

	// fonts
	auto microsoft = Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga");
	auto courierNew = Load::FNT("Files/Fonts/CourierNew.fnt", "Files/Fonts/CourierNew.tga");

	// debug text
	{
		const unsigned label = entities->Create();
		entities->GetComponent<Transform>(label)->translation.Set(-15.f, 8.f, 0.f);
		debugText = entities->AddComponent<Text>(label);
		debugText->SetActive(true);
		debugText->SetFont(courierNew);
		debugText->scale = 0.5f;
		debugText->paragraphAlignment = PARAGRAPH_LEFT;
		debugText->verticalAlignment = ALIGN_TOP;
	}

	// title
	{
		const unsigned label = entities->Create();
		titleText = entities->AddComponent<Text>(label);
		titleText->SetActive(true);
		titleText->SetFont(microsoft);
		titleText->text = "Allure 2D"; 
		titleText->color.Set(1.0f, 1.0f, 0.f, 1.0f);
	}

	//const unsigned image = entities->Create();
	//auto transform = entities->GetComponent<Transform>(image);
	//transform->translation.y = -2.0f;
	//transform->scale.Set(5.0f);
	//auto render = entities->AddComponent<Render>(image);
	//render->SetActive(true);
	//render->SetTexture("Files/Fonts/CourierNew.tga");

	// creating console
	{
		// text field
		textField = entities->Create();

		auto transform = entities->GetComponent<Transform>(textField);
		transform->translation.Set(0.0f, -5.f, -10.f);
		transform->scale.x = 15.0f;
		transform->scale.z = 0.f;

		auto render = entities->AddComponent<Render>(textField);
		render->SetActive(true);
		render->tint.Set(0.2f, 0.2f, 0.2f, 0.0f);

		auto text = entities->AddComponent<Text>(textField);
		text->SetActive(true);
		text->SetFont(courierNew);
		text->text = "Placeholder";
		text->scale = 0.5f;
		text->paragraphAlignment = PARAGRAPH_LEFT;
		text->offset.x = 0.2f;

		auto button = entities->AddComponent<Button>(textField);
		button->SetActive(true);
		button->BindHandler(MOUSE_CLICK, &TitleScene::TextFieldDidSelect, this);
	}

	{
		cursor = entities->Create();

		auto transform = entities->GetComponent<Transform>(cursor);
		transform->scale.Set(0.1f, 0.7f, 1.0f);

		auto render = entities->AddComponent<Render>(cursor);
		render->SetActive(true);
		render->tint.Set(1.0f);
	}

	Events::EventsManager::GetInstance()->Subscribe("KEY_INPUT", &TitleScene::KeyHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("TEXT_INPUT", &TitleScene::TextHandler, this);
}

void TitleScene::Update(const float & dt) {
	Scene::Update(dt);

	et += dt;

	const int FPS = static_cast<int>(1.f / dt);
	debugText->text = "FPS: " + std::to_string(FPS) + "\nCOUNT: " + std::to_string(entities->PoolCount());

	entities->GetComponent<Render>(cursor)->tint.a = abs(sin(et * 3.f));
}

void TitleScene::KeyHandler(Events::Event * event) {
	auto input = static_cast<Events::KeyInput*>(event);

	if (input->action == GLFW_PRESS && input->key == GLFW_KEY_GRAVE_ACCENT) {

	} else if (input->key == GLFW_KEY_BACKSPACE && input->action != GLFW_RELEASE) {
		auto& text = entities->GetComponent<Text>(textField)->text;
		if (text.length() && cursorPosition > 0) {
			text.erase(text.begin() + cursorPosition - 1);
			--cursorPosition;
			UpdateCursorOffset(textField);
		}
	} else if (input->key == GLFW_KEY_ENTER && input->action == GLFW_PRESS) {
		auto& text = entities->GetComponent<Text>(textField)->text;
		if (shiftHeld) {
			text.insert(text.begin() + cursorPosition, '\n');
			++cursorPosition;
			UpdateCursorOffset(textField);
		} else {
			//DidReturn(this);
			text.clear();
			cursorPosition = 0;
			UpdateCursorOffset(textField);
		}
	} else if (input->key == GLFW_KEY_LEFT && input->action != GLFW_RELEASE) {
		if (cursorPosition != 0) {
			--cursorPosition;
			UpdateCursorOffset(textField);
		}
	} else if (input->key == GLFW_KEY_RIGHT && input->action != GLFW_RELEASE) {
		auto text = entities->GetComponent<Text>(textField);
		if (cursorPosition != text->text.length()) {
			++cursorPosition;
			UpdateCursorOffset(textField);
		}
	} else if (input->key == GLFW_KEY_LEFT_SHIFT) {
		if (input->action == GLFW_PRESS) {
			shiftHeld = true;
		} else if (input->action == GLFW_RELEASE) {
			shiftHeld = false;
		}
	}
}

void TitleScene::TextHandler(Events::Event * event) {
	auto input = static_cast<Events::TextInput*>(event);

	if (selected) {
		const auto c = static_cast<Events::TextInput*>(event)->data;
		auto& text = entities->GetComponent<Text>(textField)->text;
		text.insert(text.begin() + cursorPosition, c);
		++cursorPosition;
		UpdateCursorOffset(textField);
		//DidChange(this);
	}
}

void TitleScene::TextFieldDidSelect(unsigned target) {
	selected = true;
	cursorPosition = entities->GetComponent<Text>(textField)->text.length();
	UpdateCursorOffset(target);
}

void TitleScene::UpdateCursorOffset(unsigned target) {
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
	for (int i = 0; i < cursorPosition; ++i) {
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
