#include "ScoreController.h"

#include "ScoreEvent.h"

#include <Events/EventsManager.h>
#include <Helpers/StringHelpers.h>

void ScoreController::Awake() {
	EventsManager::Get()->Subscribe("SCORE", &ScoreController::ScoreHandler, this);
	EventsManager::Get()->Subscribe("BUILD_MULTIPLIER", &ScoreController::BuildHandler, this);
	EventsManager::Get()->Subscribe("RESET_MULTIPLIER", &ScoreController::ResetHandler, this);
}

void ScoreController::Start() {
	totalScore = 0;
	buildScore = 0;
	multiplier = 1;

	hiddenScore = 0;

	totalScoreLabel->text = Helpers::Pad(std::to_string(totalScore), 6,'0');
	buildScoreLabel->text = "";
	multiplierLabel->text = "";
}

void ScoreController::ScoreHandler(Events::Event * event) {
	const auto score = static_cast<Events::Score*>(event);
	hiddenScore += score->points;
	buildScore += hiddenScore;
	buildScoreLabel->text = Helpers::Pad(std::to_string(buildScore), 6,'0');

	Transform* const iTransform = indicatorPrefab->Create();
	iTransform->translation = score->position;
	iTransform->translation.y += 3.f;

	Text* const text = entities->GetComponent<Text>(iTransform->entity);
	text->text = std::to_string(hiddenScore);
}

void ScoreController::BuildHandler() {
	++multiplier;
	multiplierLabel->text = "x" + std::to_string(multiplier);

	vfx->multiplier = max(static_cast<float>(multiplier) - 10.f, 0.f) / 10.f;
}

void ScoreController::ResetHandler() {
	totalScore += buildScore * multiplier;
	buildScore = 0;
	multiplier = 1;
	hiddenScore = 0;

	totalScoreLabel->text = Helpers::Pad(std::to_string(totalScore), 6,'0');
	buildScoreLabel->text = "";
	multiplierLabel->text = "";

	vfx->multiplier = 0.f;
}
