#ifndef TITLE_SCENE_H
#define TITLE_SCENE_H

#include "Scene.h"

#include "Transform.h"
#include "Render.h"
#include "Camera.h"
#include "Text.h"
#include "Button.h"

#include "UITextFieldManager.h"

#include <Math/Vectors.hpp>
#include <Events/Event.h>

class TitleScene : public Scene {

	bool transition;
	float transitionDelay;

	Camera* camera;
	Transform* mouse;
	
	Text* titleText;
	Render* buttonRender;

	UITextFieldManager* textFieldManager;

public:

	void Awake() override;
	void Update(const float& dt) override;

	void Destroy() override;

private:

	// event handler

	void CursorPositionHandler(Events::Event* event);

	void OnMouseOver(unsigned target);
	void OnMouseOut(unsigned target);
	void OnMouseDown(unsigned target);
	void OnMouseUp(unsigned target);
	void OnClick(unsigned target);

	// helper methods

	void FadeOut();

};


#endif
