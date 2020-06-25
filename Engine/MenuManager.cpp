#include "MenuManager.h"

#include "InputEvents.h"
#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

MenuManager::MenuManager()
	: selectionDelay(0.5f) {
}

void MenuManager::AddSong(SongData const & data) {
	songs.push_back(data);
}

void MenuManager::NextSong() {
	++selected;
	if (selected >= static_cast<int>(songs.size()))
		selected = 0;
	SwitchingSong();
}

void MenuManager::PreviousSong() {
	--selected;
	if (selected < 0)
		selected = songs.size() - 1;
	SwitchingSong();
}

void MenuManager::Awake() {
	EventsManager::Get()->Subscribe("KEY_INPUT", &MenuManager::KeyHandler, this);
}

void MenuManager::Start() {
	bt = 0.f;
	selected = 0;
	switched = true;
}

void MenuManager::Update() {
	if (switched) {
		bt += time->dt;
		if (bt >= selectionDelay)
			UpdateSong();
	}
}

void MenuManager::KeyHandler(Events::Event * event) {
	Events::KeyInput* input = static_cast<Events::KeyInput*>(event);

	if (input->action != GLFW_RELEASE) {
		if (input->key == GLFW_KEY_LEFT || input->key == GLFW_KEY_A) {
			PreviousSong();
		} else if (input->key == GLFW_KEY_RIGHT || input->key == GLFW_KEY_D) {
			NextSong();
		}
	}
}

void MenuManager::SwitchingSong() {
	switched = true;
	bt = 0.f;
	bubble->FadeOut();
}

void MenuManager::UpdateSong() {
	switched = false;
	bt = 0.f;
	bubble->Play(songs[selected]);
}

