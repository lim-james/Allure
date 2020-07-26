#include "CoinHandler.h"

#include <Events/EventsManager.h>

void CoinHandler::OnCollisionEnter(unsigned target) {
	if (pickedUp) return;

	pickedUp = true;
	render->SetSprite(pickupSheet);
	animation->queued = "IDLE";
	animation->animations = pickupData;
	animation->animations["IDLE"].loop = false;
	animation->animations["IDLE"].completed.Bind(&CoinHandler::PickedUp, this);
}

bool CoinHandler::IsPickedUp() const {
	return pickedUp;
}

void CoinHandler::Awake() {
	EventsManager::Get()->Subscribe("KEY_INPUT", &CoinHandler::KeyHandler, this);
}

void CoinHandler::Start() {
	render = GetComponent<SpriteRender>();
	animation = GetComponent<SpriteAnimation>();

	pickedUp = false;
}

void CoinHandler::KeyHandler(Events::Event * event) {
	OnCollisionEnter(0);
}

void CoinHandler::PickedUp() {
	entities->Destroy(entity);
}
