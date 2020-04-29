#include "Script.h"

#include <Events/EventsManager.h>

void Script::Initialize() {

}

Component * Script::Clone() const {
	return new Script(*this);
}

void Script::Awake() {

}

void Script::Start() {

}

void Script::FixedUpdate() {

}

void Script::Update() {

}

void Script::Stop() {

}

void Script::OnDestroy() {

}

void Script::SetActive(bool const& state) {
	Component::SetActive(state);
	EventsManager::Get()->Trigger("SCRIPT_ACTIVE", new Events::AnyType<Script*>(this));
}
