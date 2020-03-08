#include "ScriptSystem.h"

#include <Events/EventsManager.h>
#include <Helpers/VectorHelpers.h>

ScriptSystem::ScriptSystem() {
	Events::EventsManager::GetInstance()->Subscribe("SCRIPT_ACTIVE", &ScriptSystem::ActiveHandler, this);
}

ScriptSystem::~ScriptSystem() {}

void ScriptSystem::Awake() {
	for (auto& s : scripts)
		s->Awake();
}

void ScriptSystem::Start() {
	Events::EventsManager::GetInstance()->SubscribeContext(this);
	for (auto& s : scripts)
		s->Start();
}

void ScriptSystem::FixedUpdate() {
	for (auto& s : scripts)
		s->FixedUpdate();
}

void ScriptSystem::Update() {
	for (auto& s : scripts)
		s->Update();
}

void ScriptSystem::Stop() {
	for (auto& s : scripts)
		s->Stop();
	Events::EventsManager::GetInstance()->UnsubscribeContext(this);
}

void ScriptSystem::Destroy() {
	for (auto& s : scripts)
		s->OnDestroy();
}

void ScriptSystem::ActiveHandler(Events::Event * event) {
	auto& c = static_cast<Events::AnyType<BaseScript*>*>(event)->data;

	if (c->IsActive()) {
		if (Helpers::Insert(scripts, c)) {
			c->Awake();
			c->Start();
		}
	} else {
		if (Helpers::Remove(scripts, c)) {
			c->OnDestroy();
		}
	}
}
