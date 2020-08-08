#include "GameManager.h"

#include "InputEvents.h"
// destination scene
#include "PauseScene.h"
#include "ScoreScene.h"

#include "EngineProperties.h"

#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void GameManager::Awake() {
	//EventsManager::Get()->Subscribe("KEY_INPUT", &GameManager::KeyHanlder, this);
	EventsManager::Get()->Subscribe("END_GAME_F", &GameManager::EndHanlder, this);
	EventsManager::Get()->Subscribe("END_GAME_D", &GameManager::DeadHanlder, this);
}

void GameManager::Start() {
	t = 0.f;
	endGame = false;
}

void GameManager::Update() {
	if (endGame) {
		t += time->dt;

		pixelFX->size = min(RESOLUTION * 0.5f * (endDelay - t) / endDelay, RESOLUTION * 0.1f);

		//gameManager->sceneTexture = camera->GetFramebuffer()->GetTexture();
		if (t >= endDelay) {
			endGame = false;
			ScoreScene* destination = new ScoreScene;
			//destination->sceneTexture = sceneTexture;
			destination->data.good = beatController->good;
			destination->data.great = beatController->great;
			destination->data.perfect = beatController->perfect;
			destination->data.missed = beatController->missed;
			destination->data.score = scoreController->GetTotalScore();
			EventsManager::Get()->Trigger("PRESENT_SCENE", new Events::PresentScene(destination));
		}
	}

	//if (t < fadeInDuration) {
	//	t += time->dt;
	//	//overlay->tint.a = 1.f - t / fadeInDuration;

	//	if (t >= fadeInDuration) {
	//		//overlay->tint.a = 0.f;
	//		t = 0.f;
	//	}
	//} 
}

void GameManager::KeyHanlder(Events::Event * event) {
	const auto input = static_cast<Events::KeyInput*>(event);

	if (input->key == GLFW_KEY_ESCAPE && input->action == GLFW_PRESS) {
		EventsManager::Get()->Trigger("PRESENT_SCENE", new Events::PresentScene(new PauseScene));
	}
}

void GameManager::EndHanlder() {
	if (!endGame) {
		t = 0.f;
		endGame = true;
		scoreController->enabled = false;
		uiCamera->isHidden = true;
		endLabel->text = "COMPLETE";
	}
}

void GameManager::DeadHanlder() {
	if (!endGame) {
		t = 0.f;
		endGame = true;
		scoreController->enabled = false;
		uiCamera->isHidden = true;
		endLabel->text = "DEAD";
	}
}
