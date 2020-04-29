#include "System.h"

#include <Events/EventsManager.h>

System::~System() {}

void System::Start() {
	EventsManager::Get()->SubscribeContext(this);
}

void System::Stop() {
	EventsManager::Get()->UnsubscribeContext(this);
}

