#ifndef IRWEN_SCENE_H
#define IRWEN_SCENE_H

#include "Scene.h"

#include "Graph.h"
#include "Camera.h"
#include "Transform.h"

#include <map>

class IrwenScene : public Scene {

	float bt;

	Graph graph;

	Camera* camera;

	vec2f mousePosition;
	vec2f smPosition; // screenspace mouse position
	bool mouseOver, mouseMoved, mouseDown;
	Transform* selected;

	std::map<unsigned, Node*> entityNodeMap;

	Node *start, *end;


public:

	void Awake() override;
	void Update(const float& dt) override;

private:

	void CreateNode(Node * node);
	void CreateEdge(Edge * edge);

	void CursorPositionHandler(Events::Event* event);
	void MouseButtonHandler(Events::Event* event);

	void OnMouseOverHandler(unsigned entity);
	void OnMouseOutHandler(unsigned entity);
	void OnMouseDownHandler(unsigned entity);
	void OnMouseUpHandler(unsigned entity);
	void OnClick(unsigned entity);

};

#endif