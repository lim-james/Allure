#include "BubbleManager.h"

#include "InputEvents.h"
#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

BubbleManager::BubbleManager()
	: material(nullptr)
	, audioPrefab(nullptr)
	, source(nullptr)
	, controller(nullptr)
	, file(new AudioFile<int16_t>)
	, selectionDelay(0.5f) {
}

void BubbleManager::AddSong(std::string const & path) {
	songPaths.push_back(path);
}

void BubbleManager::NextSong() {
	++selected;
	if (selected >= songPaths.size()) 
		selected = 0;
	SwitchingSong();
}

void BubbleManager::PreviousSong() {
	--selected;
	if (selected < 0) 
		selected = songPaths.size() - 1;
	SwitchingSong();
}

void BubbleManager::Awake() {
	EventsManager::Get()->Subscribe("KEY_INPUT", &BubbleManager::KeyHandler, this);
	
}

void BubbleManager::Start() {
	t = bt = 0.f;
	selected = 0;
	switched = true;
}

void BubbleManager::Update() {
	if (switched) {
		bt += time->dt;
		if (bt >= selectionDelay)
			UpdateSong();
	}

	if (!source || source->IsPaused()) return;

	t += time->dt * source->speed;
	while (t > duration) t -= duration;
	material->values = file->Spectrum(t, audioDuration * 0.001f, 1, startFrequency, endFrequency);
}

void BubbleManager::OnDestroy() {
	delete file;
}

void BubbleManager::KeyHandler(Events::Event * event) {
	Events::KeyInput* input = static_cast<Events::KeyInput*>(event);

	if (input->action != GLFW_RELEASE) {
		if (input->key == GLFW_KEY_LEFT || input->key == GLFW_KEY_A) {
			PreviousSong();
		} else if (input->key == GLFW_KEY_RIGHT || input->key == GLFW_KEY_D) {
			NextSong();
		}
	}
}

void BubbleManager::SwitchingSong() {
	switched = true;
	bt = 0.f;

	if (source) {
		controller->FadeOut();
		controller = nullptr;
		source = nullptr;
	}
}

void BubbleManager::UpdateSong() {
	switched = false;
	bt = 0.f;
	t = 0.f;

	file->Open(songPaths[selected]);
	duration = file->GetDuration();

	{
		const unsigned entity = audioPrefab->Create()->entity;
		source = entities->GetComponent<AudioSource>(entity);
		controller = entities->GetComponent<AudioController>(entity);
		controller->FadeIn(songPaths[selected]);
	}
}

