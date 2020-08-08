#ifndef SCORE_MANAGER_H
#define SCORE_MANAGER_H

#include "Script.h"
#include "ScoreData.h"

#include "Text.h"

struct ScoreManager : Script {

	using base_type = ScoreManager;

	ScoreData data;
	Text* gradeLabel;
	
private:

	void Start() override;

};

#endif
