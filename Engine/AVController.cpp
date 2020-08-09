#include "AVController.h"

#include "LoadTexture.h"
#include "InputEvents.h"
#include "ProjectDefines.h"

#include <Helpers/FileHelpers.h>
#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void AVController::OnClickBPM() {
	editingBPM = !editingBPM;
	if (editingBPM) {
		bpmLabel->color = COLOR_YELLOW;
		bt = 0.5f;
	}
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

void AVController::NextBeat() {
	beatIndex = min(beatIndex + 1, static_cast<int>(roundf(duration * bps)));
	ScrubTrack();
}

void AVController::PreviousBeat() {
	beatIndex = max(beatIndex - 1, 0);
	ScrubTrack();
}

void AVController::SetTrack(std::string const & path) {
	source->audioClip = path;
	source->Queue();
	titleLabel->text = Helpers::GetFileName(path);

	beatIndex = 0;
	duration = source->GetDuration();
	durationLabel->text = GetTimeString(duration);

	UpdateProgress();
}

void AVController::Awake() {
	EventsManager::Get()->Subscribe("KEY_INPUT", &AVController::KeyHandler, this);
	EventsManager::Get()->Subscribe("TEXT_INPUT", &AVController::TextHandler, this);
}

void AVController::Start() {
	source->loop = false;
	duration = 0.f;
	bpm = 60;
	bps = 1.f;
	editingBPM = false;
}

void AVController::Update() {
	if (!source->IsPaused()) {
		UpdateProgress();
	} else {
		playButton->SetSprite(Load::Texture2D("Files/Textures/play.png"));
	}

	if (editingBPM) {
		bt -= time->dt;
		if (bt < 0.f) {
			if (bpmLabel->color == COLOR_WHITE)
				bpmLabel->color = COLOR_YELLOW;
			else
				bpmLabel->color = COLOR_WHITE;

			bt = 0.5f;
		}
	} else {
		bpmLabel->color = COLOR_WHITE;
	}
}

void AVController::KeyHandler(Events::Event * event) {
	Events::KeyInput* const input = static_cast<Events::KeyInput*>(event);

	if (input->action == GLFW_PRESS) {
		switch (input->key) 
		{
		case GLFW_KEY_SPACE:
			if (source->audioClip == "") return;
			ToggleAudio();
			break;
		case GLFW_KEY_LEFT:
			if (source->audioClip == "") return;
			PreviousBeat();
			break;
		case GLFW_KEY_RIGHT:
			if (source->audioClip == "") return;
			NextBeat();
			break;
		case GLFW_KEY_ESCAPE:
		case GLFW_KEY_ENTER:
			editingBPM = false;
			bpmLabel->color = COLOR_WHITE;
			break;
		case GLFW_KEY_BACKSPACE:
			if (editingBPM) {
				bpm /= 10;
				UpdateBPM();
			}
			break;
		default:
			break;
		}
	} else if (input->action == GLFW_REPEAT) {
		switch (input->key)
		{
		case GLFW_KEY_LEFT:
			if (source->audioClip == "") return;
			PreviousBeat();
			break;
		case GLFW_KEY_RIGHT:
			if (source->audioClip == "") return;
			NextBeat();
			break;
		default:
			break;
		}
	}
}

void AVController::TextHandler(Events::Event * event) {
	Events::TextInput* const input = static_cast<Events::TextInput*>(event);

	if (editingBPM && isdigit(input->data)) {
		const int value = input->data - '0';
		bpm *= 10;
		bpm += value;
		UpdateBPM();
	}
}

void AVController::ScrubTrack() {
	source->SetTime(static_cast<float>(beatIndex) / bps);
	UpdateProgress();
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
	const float et = source->GetTime();

	beatIndex = static_cast<int>(roundf(et * bps));

	timeLabel->text = GetTimeString(et);
	beatLabel->text = std::to_string(beatIndex);

	progressIndicator->GetConstraint(WIDTH)->multiplier = et / duration;
}

void AVController::UpdateBPM() {
	bps = static_cast<float>(bpm) / 60.f;
	bpmLabel->text = std::to_string(bpm) + " BPM";
}
