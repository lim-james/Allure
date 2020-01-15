#ifndef MENU_SCENE_H
#define MENU_SCENE_H

#include "Scene.h"

#include "Transform.h"
#include "Camera.h"
#include "Text.h"

#include <Math/Vectors.hpp>
#include <Events/Event.h>

class MenuScene : public Scene {

	vec2f buttonSize;

	Camera* camera;
	Transform* cursor;
	Text* debugText;

public:

	void Awake() override;
	void Update(const float& dt) override;

private:

	unsigned CreateButton(const std::string& title, const vec2f& position, const vec2f& size, const vec4f& color);

	// Button handlers

	void OnMouseOverHandler(unsigned entity);
	void OnMouseOutHandler(unsigned entity);
	void OnMouseDownHandler(unsigned entity);
	void OnMouseUpHandler(unsigned entity);
	void OnClick(unsigned entity);

	// Events handlers

	void CursorPositionHandler(Events::Event* event);

};


#endif
