#ifndef TABLE_VIEW_CELL_H
#define TABLE_VIEW_CELL_H

#include "Prefab.h"
#include "Camera.h"
#include "Font.h"

struct TableViewCell : Prefab {
	Camera* uiCamera;
	Font* font;
	Transform* Create() override;
};

#endif
