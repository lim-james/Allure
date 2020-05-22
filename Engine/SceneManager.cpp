#include "SceneManager.h"

#include <Events/EventsManager.h>

SceneManager::SceneManager() {
	EventsManager().Get()->Subscribe("PRESENT_SCENE", &SceneManager::PresentHandler, this);
	EventsManager().Get()->Subscribe("DISMISS_SCENE", &SceneManager::DismissHandler, this);
}

SceneManager::~SceneManager() {
	while (!sceneStack.empty()) {
		sceneStack.top()->Destroy();
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

		if (source) {
			EventsManager::Get()->TriggerQueued();
			source->Exit();

			destination->Awake();
			destination->Create();
			destination->Enter();
			source->PrepareForSegue(destination);
		} else {
			destination->Awake();
			destination->Create();
			destination->Enter();
		}

		EventsManager::Get()->Trigger("BROADCAST_SIZE");
	}
}

void SceneManager::PresentHandler(Events::Event * event) {
	Queue(static_cast<Events::PresentScene*>(event)->data);
}

void SceneManager::DismissHandler() {
	if (sceneStack.empty()) return;

	Scene* source = sceneStack.top();
	sceneStack.pop();

	if (sceneStack.empty()) {
		EventsManager::Get()->Trigger("CLOSE_WINDOW");
	}

	EventsManager::Get()->TriggerQueued();

	Scene* destination = sceneStack.top();
	source->Exit();
	destination->Enter();
	source->PrepareForSegue(destination);

	EventsManager::Get()->Trigger("BROADCAST_SIZE");

	source->Destroy();
	delete source;
}
