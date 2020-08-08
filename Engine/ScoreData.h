#ifndef SCORE_DATA_H
#define SCORE_DATA_H

struct ScoreData {
	int health, maxHealth;
	unsigned score;
	unsigned perfect, great, good, missed;
	float duration, lifetime;
};

#endif
