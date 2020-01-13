#ifndef A_STAR_SCENE_H
#define A_STAR_SCENE_H

#include "Scene.h"

#include "Camera.h"
#include "Text.h"
#include "Render.h"

#include "Maze.h"
#include "Mouse.h"

#include <vector>

class AStarScene : public Scene {

	int gridSize;

	Camera* camera;
	Text* debugText;
	
	Maze maze;
	std::vector<Render*> grid;
	std::vector<Mouse> mouse;

	bool mPressed;

public:

	void Awake() override;

	void Update(const float& dt) override;

private:

	void KeyHandler(Events::Event* event);
	void DrawPath(Events::Event* event);

	void UpdateVision(const Mouse& mice);

	unsigned CreateTile(const float& x, const float& y);
	Mouse CreateMice(const float& x, const float& y);

	void OnMouseOverHandler(unsigned entity);
	void OnMouseOutHandler(unsigned entity);
	void OnMouseDownHandler(unsigned entity);
	void OnMouseUpHandler(unsigned entity);
	void OnClick(unsigned entity);

};

#endif
