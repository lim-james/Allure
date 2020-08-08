#include "AVController.h"

#include "LoadTexture.h"
#include "InputEvents.h"

#include <Helpers/FileHelpers.h>
#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void AVController::OnClickBPM() {

}

void AVController::ToggleAudio() {
	if (source->IsPaused()) {
		playButton->SetSprite(Load::Texture2D("Files/Textures/pause.png"));
		source->Play();
	} else {
		playButton->SetSprite(Load::Texture2D("Files/Textures/play.png"));
		source->Pause();
	}
}

void AVController::SetTrack(std::string const & path) {
	source->audioClip = path;
	titleLabel->text = Helpers::GetFileName(path);

	et = 0.f;

	UpdateProgress();
}

void AVController::Awake() {
	EventsManager::Get()->Subscribe("KEY_INPUT", &AVController::KeyHandler, this);
}

void AVController::Start() {
	et = 0.f;
}

void AVController::Update() {
	if (!source->IsPaused()) {
		et += time->dt;
		UpdateProgress();
	}
}

void AVController::KeyHandler(Events::Event * event) {
	if (source->audioClip == "") return;

	Events::KeyInput* const input = static_cast<Events::KeyInput*>(event);

	if (input->key == GLFW_KEY_SPACE && input->action == GLFW_PRESS) {
		ToggleAudio();
	}
}

std::string AVController::GetTimeString(float const & t) const {
	int seconds = static_cast<int>(roundf(t));
	int minutes = seconds / 60;
	seconds %= 60;

	const std::string sStr = (seconds < 10 ? "0" : "") + std::to_string(seconds);
	const std::string mStr = (minutes < 10 ? "0" : "") + std::to_string(minutes);

	return mStr + ":" + sStr;
}

void AVController::UpdateProgress() {
	duration = source->GetDuration();
	timeLabel->text = GetTimeString(et);
	durationLabel->text = GetTimeString(duration);
	progressIndicator->GetConstraint(WIDTH)->multiplier = et / duration;
}
