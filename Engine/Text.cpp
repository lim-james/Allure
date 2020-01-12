#include "Text.h"

#include <Events/EventsManager.h>

Text::Text()
	: text("")
	, color(vec3f(0.f), 1.f)

	, font(nullptr)
	, scale(1.f)
	, lineSpacing(1.f)
	, characterSpacing(1.f)

	, paragraphAlignment(PARAGRAPH_CENTER)
	, verticalAlignment(ALIGN_MIDDLE) {}

void Text::Initialize() {
	text = "";
	color.Set(0.f, 0.f, 0.f, 1.f);

	font = nullptr;
	scale = 1.f;
	lineSpacing = 1.f;
	characterSpacing = 1.f;

	paragraphAlignment = PARAGRAPH_CENTER;
	verticalAlignment = ALIGN_MIDDLE;
}

void Text::SetActive(const bool& state) {
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Trigger("TEXT_ACTIVE", new Events::AnyType<Text*>(this));
}

Font * const Text::GetFont() const {
	return font;
}

void Text::SetFont(Font * const _font) {
	Events::FontChange* event = new Events::FontChange(font, this);
	font = _font;
	Events::EventsManager::GetInstance()->Trigger("TEXT_FONT", event);
}
