#ifndef SCORE_SCENE_H
#define SCORE_SCENE_H

#include "Scene.h"

#include "ScoreData.h"
#include "LoadFNT.h"

#include "CircleMaterial.h"

class ScoreScene : public Scene {

public:

	std::string mapPath;
	unsigned sceneTexture;
	ScoreData data;

private:

	Material::Circle* circle;

	void Awake() override;
	void Create() override;
	void Destroy() override;

	void CreateRow(float const& i, std::string const& label, std::string const& content, Font* font);

};

#endif
