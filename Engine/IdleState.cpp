#include "IdleState.h"

#include <Logger/Logger.h>

void States::Idle::Enter(const unsigned& target, EntityManager * const entities) {
	Console::Warn << target << " entered idle\n";
}

void States::Idle::Update(const unsigned& target, const float& dt, EntityManager * const entities) {

}

void States::Idle::Exit(const unsigned& target, EntityManager * const entities) {
	Console::Warn << target << " exited idle\n";
}
