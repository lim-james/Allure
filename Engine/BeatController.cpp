#include "BeatController.h"

#include "Text.h"
#include "InputEvents.h"
#include "ProjectDefines.h"

#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void BeatController::SetTempo(unsigned const & tempo) {
	delay = 60.f / static_cast<float>(tempo);
}

void BeatController::Awake() {
	EventsManager::Get()->Subscribe("HIT_BEAT", &BeatController::HitHandler, this);
}

void BeatController::Start() {
	et = beatCount = 0.f;

	perfect = great = good = missed = 0;

	file = new AudioFile<int16_t>;
	file->Open(source->audioClip);
	duration = file->GetDuration();

	bt = perfectThreshold;
	isHit = false;
	endCycle = false;

	promptLabel->text = "SHOOT TO START THE MUSIC";
}

void BeatController::Update() {
	background->playerPosition = transform->GetWorldTranslation();
	background->interval = delay;
	background->threshold = perfectThreshold;

	if (source->IsPaused()) return;

	const float dt = time->dt * source->speed;

	// calculate et
	et += dt;
	while (et > duration)
		et -= duration;
	background->et = et;

	while (!background->spreads.empty()) {
		Material::Background::Spread& first = background->spreads[0];
		if (et - first.et > 2.f) {
			background->spreads.erase(background->spreads.begin());
		} else {
			break;
		}
	}

	// calculate tempo
	bt -= dt;
	if (bt < 0.f) {
		++beatCount;
		bt = delay + bt;
		endCycle = true;
		EventsManager::Get()->Trigger("BEAT");
	}
	background->bt = bt;

	// check if hit
	if (endCycle && perfectThreshold < delay - bt) {
		if (!isHit) {
			++missed;
			Alert("MISSED", COLOR_RED);
			EventsManager::Get()->Trigger("RESET_MULTIPLIER");
		}
		endCycle = false;
		isHit = false;
	}
}

void BeatController::FixedUpdate() {
	if (source->IsPaused()) return;

	//const auto sample = file->Spectrum(et, audioDuration * 0.001f, frequencyBands, startFrequency, endFrequency);
	const auto sample = file->Spectrum(et, audioDuration * 0.001f, startFrequency, endFrequency);
	const float result = static_cast<float>(sample[2].y) * 250000.f;
	EventsManager::Get()->Trigger("BEAT_VALUE", new Events::AnyType<float>(result));
	*meterHeight = maxHeight * result;
}

void BeatController::Stop() {
	source->Pause();
}

void BeatController::HitHandler(Events::Event * event) {
	bool * const state = static_cast<Events::AnyType<bool*>*>(event)->data;
	if (source->IsPaused()) {
		promptLabel->text = "";
		source->Play();
		EventsManager::Get()->Trigger("PLAY");

		et = 0.f;
		isHit = true;
		*state = true;
		return;
	} else if (!isHit) {
		std::string message = "";
		vec3f color;
		if (perfectThreshold >= bt || perfectThreshold >= delay - bt) {
			++perfect;
			message = "PERFECT";
			color = COLOR_PURPLE;
		} else if (greatThreshold >= bt) {
			++great;
			message = "GREAT";
			color = COLOR_YELLOW;
		} else if (goodThreshold >= bt) {
			++good;
			message = "GOOD";
			color = 1.f;
		}

		if (message != "") {
			Alert(message, color);
			background->spreads.push_back(
				Material::Background::Spread{ et, color }
			);
			isHit = true;
			*state = true;
			return;
		}
	}

	//Alert("TOO EARLY", RED);
	*state = false;
	EventsManager::Get()->Trigger("RESET_MULTIPLIER");
}

void BeatController::Alert(std::string const & message, vec3f const & color) {
	Transform* const indicator = indicatorPrefab->Create();
	indicator->SetLocalTranslation(transform->GetWorldTranslation() + vec3f(0.f, 3.f, 0.f));

	Text* const text = entities->GetComponent<Text>(indicator->entity);
	text->text = message;
	text->color.rgb = color;
}
