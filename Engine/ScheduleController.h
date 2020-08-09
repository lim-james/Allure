#ifndef SCHEDULE_CONTROLLER_H
#define SCHEDULE_CONTROLLER_H

#include "Script.h"

// utils
#include "Material.h"
#include "EnemyData.h"
// components
#include "Animator.h"
#include "SpriteRender.h"
// scripts
#include "AVController.h"

#include <Math/Vectors.h>
#include <vector>
#include <map>

struct ScheduleController : Script {

	using base_type = ScheduleController;

	AVController* avController;

	Animator* animator;
	Transform* beltTransform;
	Transform* backgroundTransform;

	void AddEnemy(EnemyPreviewData const& data);

	void IndexChangeHandler(int index);

private:

	std::vector<EnemyPreviewData> enemies;
	std::map<int, std::map<unsigned, std::vector<vec3f>>> schedule;

	std::vector<SpriteRender*> backs;
	std::vector<unsigned> currentBoard;

	int selected, beatIndex;

	void Awake() override;
	void Start() override;

	void TextHandler(Events::Event* event);

	void Select(int const& index);
	
	Transform* Create(int const& index, unsigned const& layer, vec3f const& position);

	void Spawn(int const& index);
	void Spawn(int const& index, vec3f const& position);

	void UpdateBoard();

};

#endif
