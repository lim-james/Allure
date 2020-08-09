#ifndef EDITOR_SCENE_H
#define EDITOR_SCENE_H

#include "Scene.h"

// materials
#include "CircleMaterial.h"
// prefabs
#include "AVButton.h"
// guns
#include "PistolPreview.h"
#include "AutomaticPreview.h"
#include "SniperPreview.h"
#include "ShotgunPreview.h"
#include "GrenadeLauncherPreview.h"
#include "LaserPreview.h"
// enemies
#include "BasicPreview.h"
#include "BatPreview.h"
#include "FireElementalPreview.h"
#include "IceElementalPreview.h"
#include "CyclopsPreview.h"

class EditorScene : public Scene {

	// materials
	Material::Circle* circle;

	// prefabs
	AVButton* avButton;

	// guns
	PistolPreview* pistol;
	AutomaticPreview* automatic;
	ShotgunPreview* shotgun;
	SniperPreview* sniper;
	GrenadeLauncherPreview* grenadeLauncher;
	LaserPreview* laser;
	// enemies
	BasicPreview* basicPreview;
	BatPreview* batPreview;
	FireElementalPreview* fireElementalPreview;
	IceElementalPreview* iceElementalPreview;
	CyclopsPreview* cyclopsPreview;

	void Awake() override;
	void Create() override;
	void Destroy() override;

};

#endif
