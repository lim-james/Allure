#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "Scene.h"

#include "Player.h"
#include "Camera.h"

#include "LuaScript.h"

#include <map>

class GameScene : public Scene {

	float bt;

	Graph graph;

	Camera* camera;

	vec2f mousePosition;
	vec2f smPosition; // screenspace mouse position
	bool mouseOver, mouseMoved, mouseDown;
	Transform* selected;

	std::map<unsigned, Node*> entityNodeMap;

	Node *start, *end;

	LuaScript* script;

	unsigned textField;
	unsigned cursor;
	bool selection;
	int cursorPos;
	bool isCaps;

	Player player = Player::GetInstance();
	bool returnClosest;
public:

	void Awake() override;
	void Update(const float& dt) override;
	void FixedUpdate(const float& dt) override;


private:

	void TextFieldDidSelect(unsigned target);
	void UpdateCursorOffset(unsigned target);

	void CreateNode(Node * node);
	void CreateEdge(Edge * edge);

	void TextHandler(Events::Event* event);

	void KeyHandler(Events::Event* event);
	void CursorPositionHandler(Events::Event* event);
	void MouseButtonHandler(Events::Event* event);

	void OnMouseOverHandler(unsigned entity);
	void OnMouseOutHandler(unsigned entity);
	void OnMouseDownHandler(unsigned entity);
	void OnMouseUpHandler(unsigned entity);
	void OnClick(unsigned entity);

	void UpdateNodePosition();

};

#endif