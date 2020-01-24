#ifndef TEAM_H
#define TEAM_H

#include "HexGrid.h"
#include "Unit.h"
#include "EntityManager.h"

class Team {

	HexGrid* maze;

	//HexGrid* vision;
	std::vector<bool> vision;
	std::vector<bool> visited;

	std::vector<Unit*> units;
	Unit* selected;

public:

	Team();

	void SetMaze(HexGrid * const _maze);

	void SetVision(const unsigned& size);
	const std::vector<bool>& GetVision() const;

	bool InSight(const vec2f& screenPosition) const;

	void AddUnit(Unit * const unit);
	const std::vector<Unit*>& GetUnits() const;
	Unit * const GetUnitAt(const vec3f& position) const;

	bool SelectUnit(const unsigned& entity);
	bool DestroyUnit(const unsigned& entity);
	bool DestroyUnit(Unit * const unit);
	Unit * const GetSelectedUnit() const;

	bool Move(const float& dt, EntityManager * const entities);

private:

	void Scan(Unit * const unit);

};

#endif
