#ifndef TEXT_H
#define TEXT_H

#include "Component.h"
#include "Font.h"

#include <Events/Event.h>
#include <string>

#define PARAGRAPH_LEFT		0
#define PARAGRAPH_CENTER	1
#define PARAGRAPH_RIGHT		2

#define ALIGN_TOP		0
#define ALIGN_MIDDLE	1
#define ALIGN_BOTTOM	2

struct Text;

namespace Events {

	struct FontChange : Event {
		Font* previous;
		Text* component;

		FontChange(Font* const previous, Text* const component)
			: previous(previous)
			, component(component) {}
	};

}

struct Text : Component {

	using base_type = Text;

	std::string text;
	vec4f color;

	float scale;
	float lineSpacing;
	float characterSpacing;

	short paragraphAlignment;
	short verticalAlignment;

	vec3f offset;

	Text();

	// component abstract methods

	void Initialize() override;
	Component* Clone() const override;
	void SetActive(bool const& state) override;

	// AEObject methods

	void Unpack(AENotation const& data) override;
	void Pack(AENotation& data) override;

	// text methods

	Font * const GetFont() const;
	void SetFont(Font * const _font);

private:

	Font* font;

};

#endif
