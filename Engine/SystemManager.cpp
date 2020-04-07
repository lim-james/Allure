#include "SystemManager.h"

#include <Events/EventsManager.h>

SystemManager::SystemManager(EntityManager * const manager) : manager(manager) {}

SystemManager::~SystemManager() {
	for (auto& pair : systems)
		delete pair.second;
}

void SystemManager::Start() {
	frame = frameLayout.begin();
	for (auto& systemPair : systems)
		Events::EventsManager::GetInstance()->SubscribeContext(systemPair.second);
}

void SystemManager::Stop() {
	for (auto& systemPair : systems)
		Events::EventsManager::GetInstance()->UnsubscribeContext(systemPair.second);
}

void SystemManager::Update(float const& dt) {
	for (auto& frame : frameDelta) {
		frame.second += dt;
	}

	for (auto& s : *frameLayout.begin()) {
		s->Update(dt);
	}

	if (frameLayout.size() <= 1) return;

	for (auto& s : *frame) {
		s->Update(frameDelta[s]);
		frameDelta[s] = 0.f;
	}

	++frame;
	if (frame == frameLayout.end()) {
		frame = frameLayout.begin();
		++frame;
	}
}
