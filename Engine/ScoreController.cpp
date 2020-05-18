#include "ScoreController.h"

#include "ScoreEvent.h"

#include <Events/EventsManager.h>

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

	totalScoreLabel->text = std::to_string(totalScore);
	buildScoreLabel->text = "";
	multiplierLabel->text = "";
}

void ScoreController::ScoreHandler(Events::Event * event) {
	const auto score = static_cast<Events::Score*>(event);
	hiddenScore += score->points;
	buildScore += hiddenScore;
	buildScoreLabel->text = std::to_string(buildScore);

	Transform* const iTransform = indicatorPrefab->Create();
	iTransform->translation = score->position;
	iTransform->translation.y += 3.f;

	Text* const text = entities->GetComponent<Text>(iTransform->entity);
	text->text = std::to_string(hiddenScore);
}

void ScoreController::BuildHandler() {
	++multiplier;
	multiplierLabel->text = "x" + std::to_string(multiplier);
}

void ScoreController::ResetHandler() {
	totalScore += buildScore * multiplier;
	buildScore = 0;
	multiplier = 1;
	hiddenScore = 0;

	totalScoreLabel->text = std::to_string(totalScore);
	buildScoreLabel->text = "";
	multiplierLabel->text = "";
}
