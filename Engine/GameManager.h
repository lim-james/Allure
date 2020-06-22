#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "Script.h"

#include "SpriteRender.h"

struct GameManager : Script {

	using base_type = GameManager;

	float fadeInDuration;

private:

	float t; 

	void Awake() override;
	void Start() override;
	void Update() override;

	void KeyHanlder(Events::Event* event);

};

#endif
