#ifndef TILEMAP_EDITOR_MANAGER_H
#define TILEMAP_EDITOR_MANAGER_H

#include "Script.h"

#include "TilemapRender.h"
#include "TilemapCamera.h"
#include "SpriteRender.h"
#include "Text.h"

#include "TilemapLitMaterial.h"

#include "TilemapResource.h"

struct TilemapEditorManager : Script {

	using base_type = TilemapEditorManager;

	Material::Base* tilemapLit;

	TilemapCamera* editorCamera;
	Camera* uiCamera;

	Text* layerLabel;
	Text* sizeLabel;

	SpriteRender* grid;
	void SetPreview(SpriteRender* const render);

	void PreviewMouseOver();
	void PreviewMouseOut();
	void PreviewClick();

private:

	int layerHeight;
	std::map<int, TilemapRender*> tilemaps;

	vec2f cursorPosition;
	vec2f cursorSelection;

	SpriteRender* previewRender;
	Transform* previewTransform;

	unsigned index, size;

	TilemapPalette palette;
	
	void Awake() override;
	void Start() override;

	void DropEvent(Events::Event* event);
	void KeyEvent(Events::Event* event);
	void CursorPositionHandler(Events::Event* event);
	void MouseButtonHandler(Events::Event* event);

	void UpdateIndex();

	void CreateLayer(int const& z);

};

#endif
