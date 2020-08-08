#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "Script.h"

#include "Camera.h"
#include "Text.h"
#include "BeatController.h"
#include "ScoreController.h"
#include "Pixelated.h"
#include "Vignette.h"

struct GameManager : Script {

	using base_type = GameManager;

	float fadeInDuration;
	float endDelay;

	Camera* uiCamera;
	Text* endLabel;

	BeatController* beatController;
	ScoreController* scoreController;

	Pixelated* pixelFX;
	Vignette* vignetteFX;

private:

	float t; 
	bool endGame;

	void Awake() override;
	void Start() override;
	void Update() override;

	void KeyHanlder(Events::Event* event);
	void EndHanlder();
	void DeadHanlder();

};

#endif
