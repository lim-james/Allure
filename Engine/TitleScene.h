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

	std::string destination;
	float transitionDelay;
	float splashDelay;

	Camera* camera;
	Transform* mouse;
	
	Text* titleText;
	Render* buttonRender;

	std::string file;

public:

	void Awake() override;
	void Create() override;
	void Update(const float& dt) override;

private:

	// event handler

	void CursorPositionHandler(Events::Event* event);
	void DropHandler(Events::Event* event);

	void OnMouseOver(unsigned target);
	void OnMouseOut(unsigned target);
	void OnMouseDown(unsigned target);
	void OnMouseUp(unsigned target);
	void OnClick(unsigned target);

	// helper methods

	void FadeOut();

};


#endif
