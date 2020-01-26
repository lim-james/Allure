#ifndef TEAM_H
#define TEAM_H

#include "HexGrid.h"
#include "Unit.h"
#include "EntityManager.h"

#include <functional>

class Team {

	std::string name;

	HexGrid* maze;

	//HexGrid* vision;
	std::vector<bool> vision;
	std::vector<bool> visited;

	std::vector<Unit*> units;
	Unit* selected;

public:

	Team();

	void SetName(const std::string& _name);
	const std::string& GetName() const;

	void SetMaze(HexGrid * const _maze);

	void SetVision(const unsigned& size);
	const std::vector<bool>& GetVision() const;

	bool InSight(const vec2f& screenPosition) const;

	void AddUnit(Unit * const unit);
	const std::vector<Unit*>& GetUnits() const;
	Unit * const GetUnitAt(const vec3f& position) const;

	bool IsDead() const;

	bool SelectUnit(const unsigned& entity);
	Unit * const GetSelectedUnit() const;

	bool DestroyUnit(const unsigned& entity);
	bool DestroyUnit(Unit * const unit);
	void DestroyUnits(const std::function<void(unsigned)>& completion);

	std::vector<vec2i> GetPath(const vec2f& start, const vec2f& end) const;

	bool Move(const float& dt, EntityManager * const entities);

private:

	void Scan(Unit * const unit);

};

#endif
