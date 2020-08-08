#ifndef EDITOR_SCENE_H
#define EDITOR_SCENE_H

#include "Scene.h"

// materials
#include "CircleMaterial.h"
// prefabs
#include "AVButton.h"

class EditorScene : public Scene {

	// materials
	Material::Circle* circle;

	// prefabs
	AVButton* avButton;

	void Awake() override;
	void Create() override;
	void Destroy() override;

};

#endif
