#ifndef EDITOR_MANAGER_H
#define EDITOR_MANAGER_H

#include "Script.h"

#include "TilemapRender.h"
#include "EditorCamera2D.h"
#include "SpriteRender.h"
#include "Text.h"

#include "TilemapResource.h"

struct EditorManager : Script {

	using base_type = EditorManager;

	TilemapRender* tilemap;

	EditorCamera2D* editorCamera;
	Camera* uiCamera;

	Text* sizeLabel;

	SpriteRender* grid;
	void SetPreview(SpriteRender* const render);

	void PreviewMouseOver();
	void PreviewMouseOut();
	void PreviewClick();

private:

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

};

#endif
