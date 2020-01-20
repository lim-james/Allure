#ifndef HEX_SCENE_H
#define HEX_SCENE_H

#include "Scene.h"

#include "Camera.h"
#include "Text.h"
#include "Render.h"
#include "Transform.h"

#include "HexMaze.h"

#include "Team.h"
#include "Unit.h"

#include <vector>

class HexScene : public Scene {

	float moveDelay;
	float bt;

	int gridSize;

	Camera* camera;
	Text* debugText;
	Transform* highlight;

	HexMaze* maze;
	std::vector<Render*> grid;
	std::vector<vec2i> path;
	
	unsigned turnCount, moveCount;

	Team teams[2];
	bool playerTurn;

	bool mPressed;

public:

	void Awake() override;

	void Update(const float& dt) override;

private:

	void KeyHandler(Events::Event* event);
	void DrawPath(Events::Event* event);

	void UpdateVision();

	unsigned CreateTile(const int& x, const int& y);
	Unit * CreateUnit(const int& x, const int& y, const float& range);
	//HexMouse CreateMice(const float& x, const float& y);

	void OnMouseOverHandler(unsigned entity);
	void OnMouseOutHandler(unsigned entity);
	void OnMouseDownHandler(unsigned entity);
	void OnMouseUpHandler(unsigned entity);
	void OnClick(unsigned entity);
	
	void SelectUnitHandler(unsigned entity);

};

#endif
