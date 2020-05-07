#include "SystemManager.h"

#include <Events/EventsManager.h>

SystemManager::SystemManager(EntityManager * const manager) : manager(manager) {}

SystemManager::~SystemManager() {
	for (auto& pair : systems)
		delete pair.second;

	for (System* const s : fixedSystems)
		delete s;
}

void SystemManager::Start() {
	frame = frameLayout.begin();

	for (auto& systemPair : systems)
		systemPair.second->Start();

	for (System* const s : fixedSystems)
		s->Start();
}

void SystemManager::Stop() {
	for (auto& systemPair : systems)
		systemPair.second->Stop();

	for (System* const s : fixedSystems)
		s->Stop();
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

void SystemManager::FixedUpdate(float const & dt) {
	for (System* const s : fixedSystems)
		s->FixedUpdate(dt);
}
