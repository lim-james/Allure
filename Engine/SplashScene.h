#ifndef SPLASH_SCENE_H
#define SPLASH_SCENE_H

#include "Scene.h"

#include "Graph.h"
#include "Camera.h"
#include "Transform.h"

#include "LuaScript.h"
#include "SceneManager.h"

#include <map>

class SplashScene : public Scene {

	Camera* camera;

	unsigned int textID;
	bool doOnce01;

	float bt;
public:

	void Awake() override;
	void Update(const float& dt) override;

private:
	void KeyDownHander(Events::Event* event);
};

#endif