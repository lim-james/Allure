#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include "Script.h"

#include "Camera.h"
#include "ConeMaterial.h"
#include "SpriteRender.h"
#include "Animator.h"
#include "BeatCollisionHandler.h"

#include <vector>

struct PlayerController : Script {

	using base_type = PlayerController;

	// references

	SpriteRender* indicatorRender;

	void SetView(Camera* camera);

	void SetHealthRender(SpriteRender* const r);
	void SetShieldRender(SpriteRender* const r);

	// player properties

	void SetMaxHealth(float const& value);

	// handlers

	void OnEnterPlayer(unsigned target);
	void OnEnterRange(unsigned target);

private:

	Camera* view;
	Transform* viewTransform;

	SpriteRender* healthRender;
	Material::Cone* healthCone;
	Animator* healthAnimation;

	SpriteRender* shieldRender;
	Material::Cone* shieldCone;
	Transform* shieldTransform;

	float maxHealth;
	float healthInv;
	float health;

	bool isShielding;

	std::vector<BeatCollisionHandler*> beats;

	void Awake() override;
	void Start() override;
	void Update() override;

	void KeyHandler(Events::Event* event);
	void MouseButtonHandler(Events::Event* event);
	void CursorPositionHandler(Events::Event* event);

	// beat methods

	void BeatPress();
	void BeatHold();
	void BeatRelease();

	// wave methods

	void WavePress();
	void WaveHold();
	void WaveRelease();

	// helpers

	void UpdateHealth();
	void SetShieldState(bool const& state);

};

#endif
