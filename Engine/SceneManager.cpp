#include "SceneManager.h"

#include <Events/EventsManager.h>

SceneManager::SceneManager() {
	Events::EventsManager().GetInstance()->Subscribe("PRESENT_SCENE", &SceneManager::PresentHandler, this);
	Events::EventsManager().GetInstance()->Subscribe("DISMISS_SCENE", &SceneManager::DismissHandler, this);
}

SceneManager::~SceneManager() {
	while (!sceneStack.empty()) {
		delete sceneStack.top();
		sceneStack.pop();
	}
}

Scene* SceneManager::GetSource() const {
	if (sceneStack.empty())
		return nullptr;

	return sceneStack.top();
}

void SceneManager::Queue(Scene * const scene) {
	queuedScenes.push(scene);
	scene->Awake();
	scene->Exit();
}

void SceneManager::Present(Scene * const scene) {
	Queue(scene);
	Segue();
}

void SceneManager::Segue() {
	if (!queuedScenes.empty()) {
		Scene* source;
		if (sceneStack.empty())
			source = nullptr;
		else 
			source = sceneStack.top();

		Scene* destination = queuedScenes.front();
		queuedScenes.pop();
		sceneStack.push(destination);

		Transit(source, destination);
		destination->Create();
	}
}

void SceneManager::PresentHandler(Events::Event * event) {
	Queue(static_cast<Events::PresentScene*>(event)->data);
}

void SceneManager::DismissHandler() {
	if (sceneStack.empty()) return;

	Scene* previous = sceneStack.top();
	sceneStack.pop();
	Scene* current = sceneStack.top(); 

	Transit(previous, current);

	previous->Destroy();
	delete previous;
}

void SceneManager::Transit(Scene * const source, Scene * const destination) {
	if (source) {
		Events::EventsManager::GetInstance()->TriggerQueued();
		source->Exit();

		destination->Enter();
		source->PrepareForSegue(destination);
	} else {
		destination->Enter();
	}

	Events::EventsManager::GetInstance()->Trigger("BROADCAST_SIZE");
}

