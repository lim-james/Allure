#ifndef SCORE_MANAGER_H
#define SCORE_MANAGER_H

#include "Script.h"
#include "ScoreData.h"

#include "Animator.h"
#include "Text.h"

struct ScoreManager : Script {

	using base_type = ScoreManager;

	std::string mapPath;
	ScoreData data;
	
	Animator* highscore;
	Text* gradeLabel;
	
private:

	void Start() override;

	unsigned GetValue(std::string const& grade) const;

};

#endif
