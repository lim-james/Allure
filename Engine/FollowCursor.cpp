#include "FollowCursor.h"

#include "InputEvents.h"

#include <Events/EventsManager.h>

void FollowCursor::Awake() {
	EventsManager::Get()->Subscribe("CURSOR_POSITION_INPUT", &FollowCursor::CursorPositionHandler, this);
}

void FollowCursor::CursorPositionHandler(Events::Event * event) {
	auto input = static_cast<Events::CursorPositionInput*>(event);

	auto position = camera->ScreenToWorldPosition(input->position);
	position.y = -position.y;
	transform->SetLocalTranslation(position);
}
