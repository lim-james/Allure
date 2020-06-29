#ifndef SCORE_SCENE_H
#define SCORE_SCENE_H

#include "Scene.h"

#include "ScoreData.h"
#include "LoadFNT.h"

class ScoreScene : public Scene {

public:

	unsigned sceneTexture;
	ScoreData data;

private:
	
	void Awake() override;
	void Create() override;

	void CreateRow(float const& i, std::string const& label, std::string const& content, Font* font);

};

#endif
