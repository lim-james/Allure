#include "UITextFieldManager.h"

// components
#include "Transform.h"
#include "SpriteRender.h"
#include "Text.h"
#include "Button.h"
// evnets
#include "InputEvents.h"

#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

UITextFieldManager::UITextFieldManager(EntityManager * const entities)
	: entities(entities)
	, et(0.f)
	, active(0) {
	cursor = entities->Create();

	auto transform = entities->GetComponent<Transform>(cursor);
	transform->scale = vec3f(0.1f, 0.7f, 1.0f);

	auto render = entities->AddComponent<SpriteRender>(cursor);
	render->SetActive(true);
	render->tint = vec4f(1.0f);

	Events::EventsManager::GetInstance()->Subscribe("KEY_INPUT", &UITextFieldManager::KeyHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("TEXT_INPUT", &UITextFieldManager::TextHandler, this);
}

unsigned UITextFieldManager::Create(vec3f const& position, const vec3f size, Font * const font) {
	const unsigned textField = entities->Create();

	auto transform = entities->GetComponent<Transform>(textField);
	transform->translation = position;
	transform->scale = size;

	auto render = entities->AddComponent<SpriteRender>(textField);
	render->SetActive(true);
	render->tint = vec4f(0.2f, 0.2f, 0.2f, 0.0f);

	auto text = entities->AddComponent<Text>(textField);
	text->SetActive(true);
	text->SetFont(font);
	text->text = "Placeholder";
	text->scale = 0.5f;
	text->paragraphAlignment = PARAGRAPH_LEFT;
	text->offset.x = 0.2f;

	auto button = entities->AddComponent<Button>(textField);
	button->SetActive(true);
	button->handlers[MOUSE_CLICK].Bind(&UITextFieldManager::DidSelect, this);

	return textField;
}

void UITextFieldManager::Update(float const& dt) {
	entities->GetComponent<SpriteRender>(cursor)->tint.a = abs(sin(et * 3.f)) * static_cast<float>(active > 0);
	et += dt;
}

void UITextFieldManager::KeyHandler(Events::Event * event) {
	if (active) return;

	auto input = static_cast<Events::KeyInput*>(event);

	if (input->action == GLFW_PRESS && input->key == GLFW_KEY_GRAVE_ACCENT) {

	} else if (input->key == GLFW_KEY_BACKSPACE && input->action != GLFW_RELEASE) {
		auto& text = entities->GetComponent<Text>(active)->text;
		if (text.length() && cursorPosition > 0) {
			text.erase(text.begin() + cursorPosition - 1);
			--cursorPosition;
			UpdateCursorOffset(active);
		}
	} else if (input->key == GLFW_KEY_ENTER && input->action == GLFW_PRESS) {
		auto& text = entities->GetComponent<Text>(active)->text;
		if (shiftHeld) {
			text.insert(text.begin() + cursorPosition, '\n');
			++cursorPosition;
			UpdateCursorOffset(active);
		} else {
			//DidReturn(this);
			text.clear();
			cursorPosition = 0;
			UpdateCursorOffset(active);
		}
	} else if (input->key == GLFW_KEY_LEFT && input->action != GLFW_RELEASE) {
		if (cursorPosition != 0) {
			--cursorPosition;
			UpdateCursorOffset(active);
		}
	} else if (input->key == GLFW_KEY_RIGHT && input->action != GLFW_RELEASE) {
		auto text = entities->GetComponent<Text>(active);
		if (cursorPosition != text->text.length()) {
			++cursorPosition;
			UpdateCursorOffset(active);
		}
	} else if (input->key == GLFW_KEY_LEFT_SHIFT) {
		if (input->action == GLFW_PRESS) {
			shiftHeld = true;
		} else if (input->action == GLFW_RELEASE) {
			shiftHeld = false;
		}
	}
}

void UITextFieldManager::TextHandler(Events::Event * event) {
	if (active) return;

	auto input = static_cast<Events::TextInput*>(event);

	const auto c = static_cast<Events::TextInput*>(event)->data;
	auto& text = entities->GetComponent<Text>(active)->text;
	text.insert(text.begin() + cursorPosition, c);
	++cursorPosition;
	UpdateCursorOffset(active);
}

void UITextFieldManager::DidSelect(unsigned target){
	target = true;
	cursorPosition = entities->GetComponent<Text>(target)->text.length();
	UpdateCursorOffset(target);
}

void UITextFieldManager::UpdateCursorOffset(unsigned target) {
	auto transform = entities->GetComponent<Transform>(target);
	auto text = entities->GetComponent<Text>(target);
	auto font = text->GetFont();

	const float scale = text->scale;

	std::vector<float> lineOffset;
	float numLines = 0;
	vec2f size(0.f);

	auto const& content = text->text;

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
		char const& c = text->text[i];
		if (c == '\0') continue;

		switch (c) {
		case '\n':
			position.y -= font->lineHeight * text->lineSpacing * text->scale;
			position.x = translation.x - lineOffset[++lineNumer];
			break;
		default:
			auto const& character = font->characters[c];
			const vec3f offset = character.rect.origin * scale;

			position.x += character.xAdvance * text->characterSpacing * scale;
			break;
		}
	}

	position.z = -10.f;
	entities->GetComponent<Transform>(cursor)->translation = position;
}
