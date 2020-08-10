#ifndef STATS_MANAGER_H
#define STATS_MANAGER_H

#include <string>
#include <map>

struct ScoreStats {
	unsigned score;
	std::string grade;

	ScoreStats();
	ScoreStats(unsigned const& score, std::string const& grade);
};

class StatsManager {

	static StatsManager* instance;

public:

	static StatsManager* Instance();

	void SetFilepath(std::string const& path);

	bool Load();
	bool Save() const;

	bool HasHighscore(std::string const& map) const;
	ScoreStats const& GetHighscore(std::string const& map) const;
	void InitHighscore(std::string const& map);
	void SetHighscore(std::string const& map, ScoreStats const& score);

private:

	std::string filepath;

	std::map<std::string, ScoreStats> highscores;

	StatsManager();

};

#endif
