#ifndef SCHEDULE_CONTROLLER_H
#define SCHEDULE_CONTROLLER_H

#include "Script.h"

// utils
#include "Material.h"
#include "EnemyData.h"
// components
#include "Camera.h"
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

	Camera* mainCamera;
	Animator* animator;
	Transform* beltTransform;
	Transform* backgroundTransform;

	void AddEnemy(EnemyPreviewData const& data);

	void IndexChangeHandler(int index);
	void OnEnemySelected(unsigned target);

	void OnCanvasClick(unsigned target);

private:

	std::vector<EnemyPreviewData> enemies;
	std::map<int, std::map<unsigned, std::vector<vec3f>>> schedule;

	std::vector<SpriteRender*> backs;
	std::map<unsigned, unsigned> buttonIndex;
	std::vector<unsigned> currentBoard;

	std::vector<std::pair<int, vec3f>> queue;

	int selected, beatIndex;

	vec3f cursorPosition;

	void Awake() override;
	void Start() override;
	void Update() override;

	void TextHandler(Events::Event* event);
	void CursorPositionHandler(Events::Event* event);

	void Select(int const& index);
	
	vec3f RandomPosition() const;
	Transform* Create(int const& index, unsigned const& layer, vec3f const& position);

	//void Spawn(int const& index);
	void Spawn(int const& index, vec3f const& position);

	void UpdateBoard();

};

#endif
