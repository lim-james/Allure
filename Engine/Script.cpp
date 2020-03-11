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

void Script::SetActive(const bool & state) {
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Trigger("SCRIPT_ACTIVE", new Events::AnyType<Script*>(this));
}
