#ifndef MAIN_MENU_SCENE_H
#define MAIN_MENU_SCENE_H

#include "Scene.h"

#include "Graph.h"
#include "Camera.h"
#include "Transform.h"

#include "LuaScript.h"
#include "SceneManager.h"

#include <map>

class MainMenuScene : public Scene {

	Camera* camera;

	vec2f mousePosition;
	vec2f smPosition; // screenspace mouse position
	bool mouseOver, mouseMoved, mouseDown;
public:

	void Awake() override;
	void Update(const float& dt) override;

private:

	void ButtonFunction(unsigned id);
	void KeyDownHander(Events::Event* event);
	void CursorPositionHandler(Events::Event* event);
	void MouseButtonHandler(Events::Event* event);

	void OnMouseOverHandler(unsigned entity);
	void OnMouseOutHandler(unsigned entity);
	void OnMouseDownHandler(unsigned entity);
	void OnMouseUpHandler(unsigned entity);
	void OnClick(unsigned entity);

};

#endif