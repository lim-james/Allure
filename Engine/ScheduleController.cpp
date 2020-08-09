#include "ScheduleController.h"

#include "EnemyCombat.h"
#include "SpriteRender.h"
#include "Text.h"
#include "Layout.h"

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

void ScheduleController::Awake() {
	EventsManager::Get()->Subscribe("TEXT_INPUT", &ScheduleController::TextHandler, this);
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
}

void ScheduleController::TextHandler(Events::Event * event) {
	if (avController->IsEditingBPM()) return;
	
	Events::TextInput* const input = static_cast<Events::TextInput*>(event);

	if (isdigit(input->data)) {
		const int index = (input->data - '0') - 1;
		if (index < enemies.size())
			Select(index);
	}
}

void ScheduleController::Select(int const & index) {
	if (index == selected) {
		Spawn(index);
	} else {
		if (selected >= 0) {
			animator->Queue(AnimationBase(false, 0.15f), &backs[selected]->tint.a, 0.15f);
		}

		selected = index;
		animator->Queue(AnimationBase(false, 0.15f), &backs[selected]->tint.a, 0.5f);
	}
}

Transform * ScheduleController::Create(int const & index, unsigned const & layer, vec3f const & position) {
	EnemyPreviewData& data = enemies[index];

	Transform* const eTransform = data.prefab->CreateIn(beltTransform);
	entities->SetLayer(eTransform->entity, layer);

	eTransform->SetLocalTranslation(position);

	EnemyCombat* const combat = entities->GetComponent<EnemyCombat>(eTransform->entity);
	if (data.weaponPrefab && combat) {
		Transform* const wTransform = data.weaponPrefab->CreateIn(combat->weaponHolder);
		entities->SetLayer(wTransform->entity, layer);

		WeaponBase* const weapon = entities->GetComponent<WeaponBase>(wTransform->entity);
		wTransform->SetLocalTranslation(weapon->HoldOffset());
	}

	return eTransform;
}

void ScheduleController::Spawn(int const & index) {
	const vec3f range = backgroundTransform->GetScale() * 0.5f;

	const vec3f position = vec3f(
		Math::RandMinMax(-range.x, range.x),
		Math::RandMinMax(-range.y, range.y),
		0.f
	);

	Transform* const eTransform = Create(index, DEFAULT, position);
	currentBoard.push_back(eTransform->entity);
	schedule[beatIndex][static_cast<unsigned>(index)].push_back(position);
}

void ScheduleController::Spawn(int const & index, vec3f const & position) {
	Transform* const eTransform = Create(index, DEFAULT, position);
	currentBoard.push_back(eTransform->entity);
	schedule[beatIndex][static_cast<unsigned>(index)].push_back(position);
}

void ScheduleController::UpdateBoard() {
	for (unsigned& entity : currentBoard)
		entities->Destroy(entity);

	for (auto const& pair : schedule[beatIndex]) 
		for (vec3f const& position : pair.second) 
			Create(pair.first, DEFAULT, position);
}
