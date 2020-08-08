#include "ScoreManager.h"

void ScoreManager::Start() {
	const float completion = data.lifetime / data.duration;
	const float unit = 1.f / static_cast<float>(data.perfect + data.great + data.good + data.missed);

	float score = static_cast<float>(data.perfect) * unit;
	score += static_cast<float>(data.great) * 0.5f * unit;
	score += static_cast<float>(data.good) * 0.1f * unit;

	score *= completion;
	score *= static_cast<float>(data.health) / static_cast<float>(data.maxHealth);
	
	if (score >= 0.98f)
		gradeLabel->text = "S+";
	else if (score >= 0.8f) 
		gradeLabel->text = "S";
	else if (score >= 0.75f) 
		gradeLabel->text = "A";
	else if (score >= 0.65f) 
		gradeLabel->text = "B";
	else if (score >= 0.55f) 
		gradeLabel->text = "C";
	else if (score >= 0.45f) 
		gradeLabel->text = "D";
	else if (score >= 0.35f) 
		gradeLabel->text = "E";
	else 
		gradeLabel->text = "F";
}
