#ifndef SCORE_CONTROLLER_H
#define SCORE_CONTROLLER_H

#include "Script.h"
#include "Text.h"
#include "Prefab.h"
#include "Vignette.h"

struct ScoreController : Script {

	using base_type = ScoreController;
	
	Text* totalScoreLabel;
	Text* buildScoreLabel;
	Text* multiplierLabel;

	Prefab* indicatorPrefab;

	Vignette* vfx;

	bool enabled;

	unsigned GetTotalScore() const;

private:

	unsigned totalScore;
	unsigned buildScore;
	unsigned hiddenScore;

	unsigned multiplier;
	unsigned mBuffer; // multiplier buffer

	void Awake() override;
	void Start() override;

	void ScoreHandler(Events::Event* event);
	void BuildHandler();
	void ResetHandler();

};

#endif
