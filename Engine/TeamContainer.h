#ifndef TEAM_CONTAINER_H
#define TEAM_CONTAINER_H

#include "Component.h"
#include "Team.h"

struct TeamContainer : Component {

	Team* team;

	TeamContainer();

	void Initialize() override;

};

#endif
