#ifndef UI_TEXT_FIELD_MANAGER_H
#define UI_TEXT_FIELD_MANAGER_H

#include "EntityManager.h"
#include "Font.h"

#include <vector>

class UITextFieldManager {

	EntityManager * const entities;
	float et;

	std::vector<unsigned> textFields;
	unsigned cursor;

	unsigned active;
	int cursorPosition;
	bool shiftHeld;

public:

	UITextFieldManager(EntityManager * const entities);

	unsigned Create(const vec3f& position, const vec3f size, Font * const font);

	void Update(const float& dt);

private:

	// events handlers
	void KeyHandler(Events::Event* event);
	void TextHandler(Events::Event* event);

	// input handlers
	void DidSelect(unsigned target);

	// helper methods
	void UpdateCursorOffset(unsigned target);

};

#endif
