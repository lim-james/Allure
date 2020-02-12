#ifndef PLAYER_EDIT_SCENE
#define PLAYER_EDIT_SCENE

#include "Scene.h"

#include "Graph.h"
#include "Camera.h"
#include "Transform.h"

#include "LuaScript.h"
#include "SceneManager.h"
#include "Player.h"

#include <map>

class PlayerEditScene : public Scene {
	Camera* camera;
	Player player = Player::GetInstance();

	unsigned speedText;
	unsigned distanceText;
	unsigned alphaText;


	vec2f mousePosition;
	vec2f smPosition; // screenspace mouse position
	bool mouseOver, mouseMoved, mouseDown;

	unsigned cursor;
	bool selection;
	int cursorPos;
	bool isCaps;
	unsigned selected;
public:

	void Awake() override;
	void ProceedToGameScene(unsigned target);
	void TextFieldDidSelect(unsigned target);
	void TextHandler(Events::Event * event);
	void UpdateCursorOffset(unsigned target);
	void Update(const float& dt) override;
	void CursorPositionHandler(Events::Event * event);
	void KeyHandler(Events::Event * event);
};

#endif // !PLAYER_EDIT_SCENE
