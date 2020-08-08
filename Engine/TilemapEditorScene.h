#ifndef TILEMAP_EDITOR_SCENE_H
#define TILEMAP_EDITOR_SCENE_H

#include "Scene.h"

#include "SpriteLitMaterial.h"
#include "TilemapLitMaterial.h"

class TilemapEditorScene : public Scene {
	
	Material::SpriteLit* spriteLit;
	Material::TilemapLit* tilemapLit;

	void Awake() override;
	void Create() override;
	void Destroy() override;

};

#endif
