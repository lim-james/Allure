#ifndef SCORE_SCENE_H
#define SCORE_SCENE_H

#include "Scene.h"

#include "ScoreData.h"

class ScoreScene : public Scene {

public:

	unsigned sceneTexture;
	ScoreData score;

private:
	
	void Awake() override;
	void Create() override;

};

#endif
