#ifndef CELL_SCRIPT_H
#define CELL_SCRIPT_H

#include "Script.h"
#include "SpriteRender.h"
#include "Text.h"
#include "Animation.h"
#include "Button.h"

struct CellScript : Script {
	using base_type = CellScript;

	unsigned index;

	SpriteRender* background;
	Button* button;
	Text* titleLabel;
	Animation* animator;
};

#endif
