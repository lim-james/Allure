#include "ScoreManager.h"

#include "StatsManager.h"

void ScoreManager::Start() {
	const float completion = data.lifetime / data.duration;
	const float unit = 1.f / static_cast<float>(data.perfect + data.great + data.good + data.missed);

	float score = static_cast<float>(data.perfect) * unit;
	score += static_cast<float>(data.great) * 0.5f * unit;
	score += static_cast<float>(data.good) * 0.1f * unit;

	score *= completion;
	score *= static_cast<float>(data.health) / static_cast<float>(data.maxHealth);
	
	std::string grade = "-";

	if (score >= 0.98f)
		grade = "S+";
	else if (score >= 0.8f) 
		grade = "S";
	else if (score >= 0.75f) 
		grade = "A";
	else if (score >= 0.65f) 
		grade = "B";
	else if (score >= 0.55f) 
		grade = "C";
	else if (score >= 0.45f) 
		grade = "D";
	else if (score >= 0.35f) 
		grade = "E";
	else 
		grade = "F";

	gradeLabel->text = grade;

	StatsManager* stats = StatsManager::Instance();

	ScoreStats const& highscoreStat = stats->GetHighscore(mapPath);

	if ((highscoreStat.grade == grade && highscoreStat.score < data.score) ||
		(GetValue(grade) > GetValue(highscoreStat.grade))) {
		highscore->SetActive(true);
		stats->SetHighscore(mapPath, ScoreStats(data.score, grade));
	} 
}

unsigned ScoreManager::GetValue(std::string const & grade) const {
	if (grade == "S+") return 8;

	switch (grade[0]) {
	case 'S':
		return 7;
	case 'A':
		return 6;
	case 'B':
		return 5;
	case 'C':
		return 4;
	case 'D':
		return 3;
	case 'E':
		return 2;
	case 'F':
		return 1;
	default:
		return 0;
	}
}
