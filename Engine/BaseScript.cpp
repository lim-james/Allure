#include "BaseScript.h"

#include <Events/EventsManager.h>

void BaseScript::Initialize() {
	Debug::Log << "Initialize\n";
}

void BaseScript::Awake() {
	Debug::Log << "Awoken\n";
}

void BaseScript::Start() {
	Debug::Log << "Start\n";
}

void BaseScript::FixedUpdate() {

}

void BaseScript::Update() {

}

void BaseScript::Stop() {
	Debug::Log << "Stop\n";
}

void BaseScript::OnDestroy() {
	Debug::Log << "Destroy\n";
}

void BaseScript::SetActive(const bool & state) {
	Component::SetActive(state);
	Debug::Log << "Active\n";
	Events::EventsManager::GetInstance()->Trigger("SCRIPT_ACTIVE", new Events::AnyType<BaseScript*>(this));
}
