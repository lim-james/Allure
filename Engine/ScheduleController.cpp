#include "ScheduleController.h"

#include "EnemyCombat.h"
#include "SpriteRender.h"
#include "Text.h"
#include "Layout.h"
#include "Button.h"

#include "WeaponBase.h"
#include "Layers.h"
#include "LoadFNT.h"
#include "InputEvents.h"

#include <Math/Random.hpp>
#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void ScheduleController::AddEnemy(EnemyPreviewData const & data) {
	enemies.push_back(data);
}

void ScheduleController::IndexChangeHandler(int index) {
	beatIndex = index;
	UpdateBoard();
}

void ScheduleController::OnPreviewClicked(unsigned target) {
	const int index = buttonIndex.at(target);
	if (index < static_cast<int>(enemies.size())) {
		Select(index);
	}
}

void ScheduleController::OnEnemyClicked(unsigned target) {
	const unsigned index = entityIndex[target];
	const vec3f position = entities->GetComponent<Transform>(target)->GetWorldTranslation();

	Helpers::Remove(schedule[beatIndex][index], position);
	Helpers::Remove(currentBoard, target);
	entities->Destroy(target);
}

void ScheduleController::OnCanvasClick(unsigned target) {
	if (avController->IsInvalid()) return;

	if (selected >= 0) {
		const vec3f range = backgroundTransform->GetScale() * 0.5f;
		if (cursorPosition.x < range.x && cursorPosition.x > -range.x &&
			cursorPosition.y < range.y && cursorPosition.y > -range.y) {
			queue.push_back(std::make_pair(selected, cursorPosition));
		}
	}
}

void ScheduleController::Awake() {
	EventsManager::Get()->Subscribe("TEXT_INPUT", &ScheduleController::TextHandler, this);
	EventsManager::Get()->Subscribe("CURSOR_POSITION_INPUT", &ScheduleController::CursorPositionHandler, this);
}

void ScheduleController::Start() {
	const float size = static_cast<float>(enemies.size());
	const float halfSize = (size - 1.f) * 0.5f;

	Font* const vcrMono = Load::FNT("Files/Fonts/vcr_ocd_mono.fnt", "Files/Fonts/vcr_ocd_mono.png");

	unsigned i = 0;
	for (float x = -halfSize; x <= halfSize; ++x, ++i) {
		EnemyPreviewData& data = enemies[i];

		const vec3f position = vec3f(x * 6.f, 0.f, 0.f);

		Transform* const eTransform = data.prefab->CreateIn(beltTransform);
		eTransform->SetLocalTranslation(position);

		const vec3f ratio = 2.f / eTransform->GetScale();
		eTransform->SetScale(4.f);

		Button* const button = entities->GetComponent<Button>(eTransform->entity);
		button->scale = 5.f / 4.f;
		button->handlers[MOUSE_CLICK].Bind(&ScheduleController::OnPreviewClicked, this);

		buttonIndex[eTransform->entity] = i;

		EnemyCombat* const combat = entities->GetComponent<EnemyCombat>(eTransform->entity);
		if (data.weaponPrefab && combat) {
			Transform* const wTransform = data.weaponPrefab->CreateIn(combat->weaponHolder);
			WeaponBase* const weapon = entities->GetComponent<WeaponBase>(wTransform->entity);
			wTransform->SetScale(wTransform->GetScale() * ratio);
			wTransform->SetLocalTranslation(weapon->HoldOffset() * ratio);
		}

		Transform* bTransform = nullptr;
		{
			const unsigned back = entities->Create();
			entities->SetLayer(back, UI);

			bTransform = entities->GetComponent<Transform>(back);
			bTransform->SetParent(eTransform);
			bTransform->SetLocalTranslation(vec3f(0.f, 0.f, -1.f));
			bTransform->SetScale(5.f);

			SpriteRender* const render = entities->AddComponent<SpriteRender>(back);
			render->SetActive(true);
			render->tint.a = 0.15f;

			backs.push_back(render);
		}

		{
			const unsigned back = entities->Create();
			entities->SetLayer(back, UI);

			Layout* const layout = entities->AddComponent<Layout>(back);
			layout->SetActive(true);
			layout->AddConstraint(Constraint(CENTER_X_ANCHOR, bTransform, CENTER_X_ANCHOR, 1.f, 0.f, nullptr));
			layout->AddConstraint(Constraint(TOP_ANCHOR, bTransform, BOTTOM_ANCHOR, 1.f, -1.f, nullptr));

			Text* const text = entities->AddComponent<Text>(back);
			text->SetActive(true);
			text->SetFont(vcrMono);
			text->scale = 0.75f;
			text->text = "[" + std::to_string(i + 1) + "]";
			text->verticalAlignment = ALIGN_TOP;
		}
	}

	selected = -1;
	beatIndex = 0;

	cursorPosition = 0.f;
}

void ScheduleController::Update() {
	for (auto const& pair : queue) {
		entityIndex[Spawn(pair.first, pair.second)] = pair.first;
	}
	queue.clear();
}

void ScheduleController::TextHandler(Events::Event * event) {
	if (avController->IsEditingBPM()) return;
	
	Events::TextInput* const input = static_cast<Events::TextInput*>(event);

	if (isdigit(input->data)) {
		const int index = (input->data - '0') - 1;
		if (index < static_cast<int>(enemies.size()))
			Select(index);
	}
}

void ScheduleController::CursorPositionHandler(Events::Event * event) {
	Events::CursorPositionInput* const input = static_cast<Events::CursorPositionInput*>(event);

	const vec3f offset = entities->GetComponent<Transform>(mainCamera->entity)->GetWorldTranslation();
	cursorPosition = mainCamera->ScreenToWorldPosition(input->position) + offset.xy;
}

void ScheduleController::Select(int const & index) {
	if (index == selected) {
		queue.push_back(std::make_pair(index, RandomPosition()));
	} else {
		if (selected >= 0) {
			animator->Queue(AnimationBase(false, 0.15f), &backs[selected]->tint.a, 0.15f);
		}

		selected = index;
		animator->Queue(AnimationBase(false, 0.15f), &backs[selected]->tint.a, 0.5f);
	}
}

vec3f ScheduleController::RandomPosition() const {
	const vec3f range = backgroundTransform->GetScale() * 0.5f;
	return vec3f(
		Math::RandMinMax(-range.x, range.x),
		Math::RandMinMax(-range.y, range.y),
		0.f
	);
}

Transform * ScheduleController::Create(int const & index, unsigned const & layer, vec3f const & position) {
	EnemyPreviewData& data = enemies[index];

	Transform* const eTransform = data.prefab->Create();
	entities->SetLayer(eTransform->entity, layer);

	eTransform->SetLocalTranslation(position);

	Button* const button = entities->GetComponent<Button>(eTransform->entity);
	button->scale = 1.5f;
	button->handlers[MOUSE_CLICK].Bind(&ScheduleController::OnEnemyClicked, this);

	EnemyCombat* const combat = entities->GetComponent<EnemyCombat>(eTransform->entity);
	if (data.weaponPrefab && combat) {
		Transform* const wTransform = data.weaponPrefab->CreateIn(combat->weaponHolder);
		entities->SetLayer(wTransform->entity, layer);

		WeaponBase* const weapon = entities->GetComponent<WeaponBase>(wTransform->entity);
		wTransform->SetLocalTranslation(weapon->HoldOffset());
	}

	return eTransform;
}

unsigned ScheduleController::Spawn(int const & index, vec3f const & position) {
	Transform* const eTransform = Create(index, DEFAULT, position);
	currentBoard.push_back(eTransform->entity);
	schedule[beatIndex][static_cast<unsigned>(index)].push_back(position);
	return eTransform->entity;
}

void ScheduleController::UpdateBoard() {
	for (unsigned& entity : currentBoard)
		entities->Destroy(entity);
	currentBoard.clear();

	for (auto const& pair : schedule[beatIndex]) {
		for (vec3f const& position : pair.second) {
			Transform* const t = Create(pair.first, DEFAULT, position);
			currentBoard.push_back(t->entity);
			entityIndex[t->entity] = pair.first;
		}

	}
}
