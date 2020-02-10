#ifndef TITLE_SCENE_H
#define TITLE_SCENE_H

#include "Scene.h"

#include "Transform.h"
#include "Camera.h"
#include "Text.h"
#include "Button.h"

#include <Math/Vectors.hpp>
#include <Events/Event.h>

class TitleScene : public Scene {

	float et;

	Text* debugText;
	Text* titleText;
	
	// Console attributes
	bool selected;
	int cursorPosition;
	bool shiftHeld;
	unsigned textField;
	unsigned cursor;

public:

	void Awake() override;
	void Update(const float& dt) override;

private:

	void KeyHandler(Events::Event* event);
	void TextHandler(Events::Event* event);

	void ToggleConsole();
	void TextFieldDidSelect(unsigned target);
	void UpdateCursorOffset(unsigned target);

};


#endif
