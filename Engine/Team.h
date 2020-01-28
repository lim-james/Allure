#ifndef TEAM_H
#define TEAM_H

#include "HexGrid.h"
#include "Unit.h"
#include "EntityManager.h"

#include <stack>
#include <functional>

class Team {

	Team* opponent;
	std::map<unsigned, Unit*> opponentUnits;
	std::map<unsigned, Unit*> inViewUnits;

	unsigned ai;
	std::string name;

	HexGrid* maze;

	std::stack<vec2i> queuedPositions;

	//HexGrid* vision;
	std::vector<bool> vision;
	std::vector<bool> visited;

	std::vector<Unit*> units;
	Unit* selected;

public:

	Team();

	void SetOpponent(Team * const _opponent);

	void SetAI(const unsigned& id);
	unsigned GetAI() const;
	bool IsAI() const;

	void SetName(const std::string& _name);
	const std::string& GetName() const;

	void SetMaze(HexGrid * const _maze);
	HexGrid * const GetMaze() const;

	std::stack<vec2i>& GetQueuedPositions();

	void SetVision(const unsigned& size);
	const std::vector<bool>& GetVision() const;

	void UpdateVision();
	Unit * const GetOpponentUnit(const unsigned& index);
	std::map<unsigned, Unit*>& GetOpponentUnits();

	bool IsVisited(const unsigned& index) const;
	void SetVisited(const unsigned& index, const bool& visibility);

	bool InSight(const vec2f& screenPosition) const;

	void AddUnit(Unit * const unit);
	const std::vector<Unit*>& GetUnits() const;
	Unit * const GetUnitAt(const vec3f& position) const;

	bool IsDead() const;

	bool SelectUnit(const unsigned& entity);
	bool SelectUnit(Unit * const unit);
	Unit * const GetSelectedUnit() const;

	bool DestroyUnit(const unsigned& entity);
	bool DestroyUnit(Unit * const unit);
	void DestroyUnits(const std::function<void(unsigned)>& completion);

	std::vector<vec2i> GetPath(const vec2f& start, const vec2f& end) const;

	unsigned Move(const float& dt, EntityManager * const entities);
	void Scan(Unit * const unit, const vec2f& position);

	void MakeMove(EntityManager * const entities);

};

#endif
