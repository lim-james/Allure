#ifndef TITLE_SCENE_H
#define TITLE_SCENE_H

#include "Scene.h"

#include "Transform.h"
#include "Camera.h"
#include "Text.h"
#include "Button.h"

#include "UITextFieldManager.h"

#include <Math/Vectors.hpp>
#include <Events/Event.h>

class TitleScene : public Scene {

	Text* titleText;
	Camera* camera;
	Transform* mouse;
	
	UITextFieldManager* textFieldManager;

public:

	void Awake() override;
	void Update(const float& dt) override;

	void Destroy() override;

private:

	void CursorPositionHandler(Events::Event* event);

};


#endif
