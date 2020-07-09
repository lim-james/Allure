#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include "Script.h"

#include "Camera.h"
#include "ConeMaterial.h"
#include "SpriteRender.h"

struct PlayerController : Script {

	using base_type = PlayerController;

	// references

	SpriteRender* indicatorRender;

	void SetView(Camera* camera);

	void SetHealthRender(SpriteRender* const r);
	void SetShieldTransform(Transform* const t);

	// player properties

	void SetMaxHealth(float const& value);

	// handlers

	void OnCollisionEnter(unsigned target);

private:

	Camera* view;
	Transform* viewTransform;

	Transform* shieldTransform;
	SpriteRender* shieldRender;

	SpriteRender* healthRender;
	Material::Cone* healthCone;

	float maxHealth;
	float healthInv;
	float health;

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

};

#endif
