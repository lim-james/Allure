#ifndef TEAM_H
#define TEAM_H

#include "HexGrid.h"
#include "Unit.h"

class Team {

	HexGrid* maze;

	HexGrid* vision;
	std::vector<bool> visited;

	std::vector<Unit*> units;
	Unit* selected;

public:

	Team();

	void SetMaze(HexGrid * const _maze);

	void SetVision(const unsigned& size);
	HexGrid * const GetVision() const;

	bool InSight(const vec2f& position) const;

	void AddUnit(Unit * const unit);
	bool SelectUnit(const unsigned& entity);

	Unit * const GetSelectedUnit() const;

	bool Move();

private:

	void Scan(Unit * const unit);

};

#endif
