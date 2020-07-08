#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include "Script.h"

#include "Camera.h"
#include "ConeMaterial.h"
#include "SpriteRender.h"

struct PlayerController : Script {

	using base_type = PlayerController;

	Camera* view;
	Material::Cone* healthCone;

	SpriteRender* indicatorRender;
	
	void SetShieldTransform(Transform* const t);

private:

	Transform* shieldTransform;
	SpriteRender* shieldRender;


	void Awake() override;
	//void Start() override;
	//void Update() override;

	void KeyHandler(Events::Event* event);
	void MouseButtonHandler(Events::Event* event);
	void CursorPositionHandler(Events::Event* event);

	void BeatPress();
	void BeatHold();
	void BeatRelease();

	void WavePress();
	void WaveHold();
	void WaveRelease();

};

#endif
