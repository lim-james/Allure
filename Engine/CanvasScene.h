#ifndef CANVAS_SCENE_H
#define CANVAS_SCENE_H

#include "Scene.h"

#include "Graph.h"

#include "Transform.h"
#include "Camera.h"
#include "ParticleEmitter.h"

#include "LuaScript.h"

#include <Events/Event.h>

#include <vector>

class CanvasScene : public Scene {

	Graph graph;

	Camera* camera;

	vec2f mousePosition;
	vec2f smPosition; // screenspace mouse position
	bool mouseOver, mouseMoved, mouseDown;
	Transform* selected;

	std::map<unsigned, Node*> entityNodeMap;
	Node *start, *end;

	struct Emitter {
		Transform * transform;
		ParticleEmitter* emitter;
		Node * node;
		std::string filepath;
		time_t lastMod;
	};

	std::vector<Emitter> emitters;

public:

	void Awake() override;
	void Start() override;

	void Update(const float& dt) override;
	void FixedUpdate(const float& dt) override;

private:

	// event handlers

	void CursorPositionHandler(Events::Event * event);
	void MouseButtonHandler(Events::Event * event);
	void DropHandler(Events::Event * event);

	void OnMouseOverHandler(unsigned entity);
	void OnMouseOutHandler(unsigned entity);
	void OnMouseDownHandler(unsigned entity);
	void OnMouseUpHandler(unsigned entity);
	void OnClick(unsigned entity);

	// helpers methods

	void CreateNode(Node * node);
	void CreateEdge(Edge * edge);
	Transform* CreateEmitter(const std::string& filepath);

	void UpdateEmitter(ParticleEmitter* emitter, const std::string& filepath);

};

#endif
