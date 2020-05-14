#include "ScriptSystem.h"

#include <Events/EventsManager.h>
#include <Helpers/VectorHelpers.h>

ScriptSystem::ScriptSystem(EntityManager * const entities) : entities(entities) {
	EventsManager::Get()->Subscribe("SCRIPT_ACTIVE", &ScriptSystem::ActiveHandler, this);

	time.et = 0.0f;
}

ScriptSystem::~ScriptSystem() {}

void ScriptSystem::Awake() {
	started = false;
	for (auto& s : scripts) {
		Setup(s);
		s->Awake();
	}
}

void ScriptSystem::Start() {
	started = true;
	EventsManager* events = EventsManager::Get();
	events->SubscribeContext(this);

	for (auto& s : scripts) {
		events->SubscribeContext(s);
		s->Start();
	}
}

void ScriptSystem::FixedUpdate(float const& dt) {
	time.fixedDt = dt;
	for (auto& s : scripts)
		s->FixedUpdate();
}

void ScriptSystem::Update(float const& dt) {
	time.dt = dt;
	time.et += dt;

	for (int i = static_cast<int>(scripts.size()) - 1; i >= 0; --i)
		scripts[i]->Update();
}

void ScriptSystem::Stop() {
	started = false;
	EventsManager* events = EventsManager::Get();
	events->UnsubscribeContext(this);
	
	for (auto& s : scripts) {
		s->Stop();
		events->UnsubscribeContext(s);
	}
}

void ScriptSystem::Destroy() {
	for (auto& s : scripts)
		s->OnDestroy();
}

void ScriptSystem::ActiveHandler(Events::Event * event) {
	auto& c = static_cast<Events::AnyType<Script*>*>(event)->data;

	if (c->IsActive()) {
		if (Helpers::Insert(scripts, c)) {
			Setup(c);
			c->Awake();
			if (started) 
				c->Start();
		}
	} else {
		if (Helpers::Remove(scripts, c)) {
			c->OnDestroy();
		}
	}
}

void ScriptSystem::Setup(Script * const script) {
	script->time = &time;
	script->entities = entities;
	script->transform = script->GetComponent<Transform>();
}

