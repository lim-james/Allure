#ifndef PLAYER_H
#define PLAYER_H

#include "Graph.h"
#include "Transform.h"

struct Player
{
	static Player& GetInstance() {
		static Player player;
		return player;
	}
	unsigned int id;
	Transform* transform;
	float speed;
	Node* node;
	float distance;
	float alpha;
};

#endif // !PLAYER_H
