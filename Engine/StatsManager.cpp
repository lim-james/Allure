#include "StatsManager.h"

#include <Logger/Logger.h>

#include <fstream>

ScoreStats::ScoreStats() 
	: score(0)
	, grade("-") {}

ScoreStats::ScoreStats(unsigned const & score, std::string const & grade) 
	: score(score)
	, grade(grade) {}

StatsManager* StatsManager::instance = nullptr;

StatsManager * StatsManager::Instance() {
	if (!instance) instance = new StatsManager;
	return instance;
}

void StatsManager::SetFilepath(std::string const & path) {
	filepath = path;
}

bool StatsManager::Load() {
	std::ifstream ifs(filepath);

	if (!ifs.is_open()) {
		Debug::Warn << "Unable to load \"" << filepath << "\".\n";
		return false;
	}

	ifs.close();

	return true;
}

bool StatsManager::Save() const {
	std::ofstream ofs(filepath);

	if (!ofs.is_open()) {
		Debug::Warn << "Unable to load \"" << filepath << "\".\n";
		return false;
	}

	ofs.close();

	return true;
}

bool StatsManager::HasHighscore(std::string const & map) const {
	return highscores.find(map) != highscores.end();
}

ScoreStats const & StatsManager::GetHighscore(std::string const & map) const {
	return highscores.at(map);
}

void StatsManager::InitHighscore(std::string const & map) {
	highscores[map] = ScoreStats{ 0, "-" };
}

void StatsManager::SetHighscore(std::string const & map, ScoreStats const & score) {
	highscores[map] = score;
}

StatsManager::StatsManager()
	: filepath("") {}
