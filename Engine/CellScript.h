#ifndef CELL_SCRIPT_H
#define CELL_SCRIPT_H

#include "Script.h"
#include "SpriteRender.h"
#include "Text.h"
#include "Animation.h"

struct CellScript : Script {
	using base_type = CellScript;
	SpriteRender* background;
	Text* titleLabel;
	Animation* animator;
};

#endif
