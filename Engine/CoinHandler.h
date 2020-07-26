#ifndef CON_HANDLER_H
#define CON_HANDLER_H

#include "Script.h"

#include "SpriteRender.h"
#include "SpriteAnimation.h"

#include <Events/Event.h>

struct CoinHandler : Script {

	using base_type = CoinHandler;

	unsigned pickupSheet;
	SADMap pickupData;

	void OnCollisionEnter(unsigned target);

	bool IsPickedUp() const;

private:

	bool pickedUp;

	SpriteRender* render;
	SpriteAnimation* animation;

	void Awake() override;
	void Start() override;
	
	void KeyHandler(Events::Event* event);

	void PickedUp();

};

#endif
