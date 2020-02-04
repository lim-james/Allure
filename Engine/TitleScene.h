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

	Text* debugText;
	Text* titleText;

public:

	void Awake() override;
	void Update(const float& dt) override;

private:

};


#endif
