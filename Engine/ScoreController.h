#ifndef SCORE_CONTROLLER_H
#define SCORE_CONTROLLER_H

#include "Script.h"
#include "Text.h"
#include "Prefab.h"

struct ScoreController : Script {
	
	Text* totalScoreLabel;
	Text* buildScoreLabel;
	Text* multiplierLabel;

	Prefab* indicatorPrefab;

private:

	unsigned totalScore;
	unsigned buildScore;
	unsigned multiplier;

	unsigned hiddenScore;

	void Awake() override;
	void Start() override;

	void ScoreHandler(Events::Event* event);
	void BuildHandler();
	void ResetHandler();

};

#endif
