#include "ScoreController.h"

#include "ScoreEvent.h"

#include <Events/EventsManager.h>
#include <Helpers/StringHelpers.h>

unsigned ScoreController::GetTotalScore() const {
	return totalScore + buildScore * multiplier;
}

void ScoreController::Awake() {
	EventsManager::Get()->Subscribe("SCORE", &ScoreController::ScoreHandler, this);
	EventsManager::Get()->Subscribe("BUILD_MULTIPLIER", &ScoreController::BuildHandler, this);
	EventsManager::Get()->Subscribe("RESET_MULTIPLIER", &ScoreController::ResetHandler, this);
}

void ScoreController::Start() {
	totalScore = 0;
	buildScore = 0;
	hiddenScore = 0;

	multiplier = 1;
	mBuffer = 1;

	totalScoreLabel->text = Helpers::Pad(std::to_string(totalScore), 6,'0');
	buildScoreLabel->text = "";
	multiplierLabel->text = "";

	enabled = true;
}

void ScoreController::ScoreHandler(Events::Event * event) {
	if (!enabled) return;

	const auto score = static_cast<Events::Score*>(event);
	hiddenScore += score->points;
	buildScore += hiddenScore;
	buildScoreLabel->text = Helpers::Pad(std::to_string(buildScore), 6,'0');

	vec3f position = score->position;
	position += 3.f;

	Transform* const iTransform = indicatorPrefab->Create();
	iTransform->SetLocalTranslation(position);

	Text* const text = entities->GetComponent<Text>(iTransform->entity);
	text->text = std::to_string(hiddenScore);
}

void ScoreController::BuildHandler() {
	if (!enabled) return;

	if (--mBuffer == 0) {
		mBuffer = ++multiplier;
		multiplierLabel->text = "x" + std::to_string(multiplier);

		vfx->multiplier = min(max(static_cast<float>(multiplier) - 5.f, 0.f) / 10.f, 1.f);
	}
}

void ScoreController::ResetHandler() {
	if (!enabled) return;

	totalScore += buildScore * multiplier;
	buildScore = 0;
	hiddenScore = 0;

	multiplier = 1;
	mBuffer = 1;

	totalScoreLabel->text = Helpers::Pad(std::to_string(totalScore), 6,'0');
	buildScoreLabel->text = "";
	multiplierLabel->text = "";

	vfx->multiplier = 0.f;
}
