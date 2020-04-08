#include "System.h"

#include <Events/EventsManager.h>

System::~System() {}

void System::Start() {
	Events::EventsManager::GetInstance()->SubscribeContext(this);
}

void System::Stop() {
	Events::EventsManager::GetInstance()->UnsubscribeContext(this);
}

