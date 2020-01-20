#ifndef TEAM_H
#define TEAM_H

#include "HexGrid.h"
#include "Unit.h"

class Team {

	HexGrid* maze;

	HexGrid* vision;
	std::vector<bool> visited;

	std::vector<Unit*> units;

public:

	void SetMaze(HexGrid * const _maze);

	void SetVision(const unsigned& size);
	HexGrid * const GetVision() const;

	void AddUnit(Unit * const unit);

private:

	void Scan(Unit * const unit);

};

#endif
