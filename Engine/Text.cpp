#include "Text.h"

#include <Events/EventsManager.h>

Text::Text()
	: text("")
	, color(1.f)

	, font(nullptr)
	, scale(1.f)
	, lineSpacing(1.f)
	, characterSpacing(1.f)

	, paragraphAlignment(PARAGRAPH_CENTER)
	, verticalAlignment(ALIGN_MIDDLE) 

	, offset(0.f) {}

void Text::Initialize() {
	text = "";
	color = vec4f(1.f);

	font = nullptr;
	scale = 1.f;
	lineSpacing = 1.f;
	characterSpacing = 1.f;

	paragraphAlignment = PARAGRAPH_CENTER;
	verticalAlignment = ALIGN_MIDDLE;

	offset = vec3f(0.f);
}

Component * Text::Clone() const {
	return new Text(*this);
}

void Text::SetActive(bool const& state) {
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Trigger("TEXT_ACTIVE", new Events::AnyType<Text*>(this));
}

void Text::Unpack(AENotation const& data) {
	Component::Unpack(data);

	text = data.Get("text");
	color = data.Get<vec4f>("color");

	scale = data.Get<float>("scale");
	lineSpacing = data.Get<float>("lineSpacing");
	characterSpacing = data.Get<float>("characterSpacing");

	paragraphAlignment = data.Get<short>("paragraphAlignment");
	verticalAlignment = data.Get<short>("verticalAlignment");

	offset = data.Get<vec3f>("offset");
}

void Text::Pack(AENotation & data) {
	data.Set("type", "Text");
	Component::Pack(data);

	data.Set("text", text);
	data.Set("color", color);

	data.Set("scale", scale);
	data.Set("lineSpacing", lineSpacing);
	data.Set("characterSpacing", characterSpacing);

	data.Set("paragraphAlignment", paragraphAlignment);
	data.Set("verticalAlignment", verticalAlignment);

	data.Set("offset", offset);
}

Font * const Text::GetFont() const {
	return font;
}

void Text::SetFont(Font * const _font) {
	Events::FontChange* event = new Events::FontChange(font, this);
	font = _font;
	Events::EventsManager::GetInstance()->Trigger("TEXT_FONT", event);
}
