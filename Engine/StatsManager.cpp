#include "StatsManager.h"

#include <Logger/Logger.h>
#include <Helpers/StringHelpers.h>

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

	unsigned count;
	ifs >> count;
	ifs.get();

	for (; count > 0; --count) {
		std::string path;
		ScoreStats stats;

		std::getline(ifs, path);
		ifs >> stats.score >> stats.grade;

		highscores[path] = stats;
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

	ofs << highscores.size() << '\n';

	for (auto& pair : highscores) {
		ofs << pair.first << '\n';
		ofs << pair.second.score << ' ' << pair.second.grade << '\n';
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
	if (Helpers::Trim(map) == "") return;
	highscores[map] = ScoreStats{ 0, "-" };
	Save();
}

void StatsManager::SetHighscore(std::string const & map, ScoreStats const & score) {
	if (Helpers::Trim(map) == "") return;
	highscores[map] = score;
	Save();
}

StatsManager::StatsManager()
	: filepath("") {}
