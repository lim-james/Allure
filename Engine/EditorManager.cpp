#include "EditorManager.h"

#include "InputEvents.h"
#include "LoadTilemap.h"

#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void EditorManager::SetPreview(SpriteRender * const render) {
	previewRender = render;
	previewTransform = entities->GetComponent<Transform>(render->entity);
}

void EditorManager::PreviewMouseOver() {
	editorCamera->enabled = false;
}

void EditorManager::PreviewMouseOut() {
	editorCamera->enabled = true;
}

void EditorManager::PreviewClick() {
	const TilemapTexture tmt = palette.GetTexture(index);
	const vec2f size = tmt.GetDimensions();

	const vec2f previewOffset = previewTransform->GetWorldTranslation().xy;
	const vec2f previewSize = previewTransform->GetScale().xy;

	const vec2f screenPosition = uiCamera->ScreenToWorldPosition(cursorPosition);
	const vec2f offset = (screenPosition - previewOffset + previewSize * 0.5f) / previewSize;
	cursorSelection = offset * size;
	cursorSelection.x = floorf(cursorSelection.x);
	cursorSelection.y = floorf(cursorSelection.y);

	SpriteRender* const cursor = entities->GetComponent<SpriteRender>(editorCamera->cursor->entity);
	cursor->tint = 1.f;

	cursor->SetSprite(tmt.texture);
	cursor->SetTilemapSize(size.x, size.y);
	cursor->SetCellRect(cursorSelection.x, cursorSelection.y, 1.f, 1.f);

	cursorSelection.y = size.y - cursorSelection.y - 1.f;
}

void EditorManager::Awake() {
	EventsManager::Get()->Subscribe("DROP_INPUT", &EditorManager::DropEvent, this);
	EventsManager::Get()->Subscribe("KEY_INPUT", &EditorManager::KeyEvent, this);
	EventsManager::Get()->Subscribe("CURSOR_POSITION_INPUT", &EditorManager::CursorPositionHandler, this);
	EventsManager::Get()->Subscribe("MOUSE_BUTTON_INPUT", &EditorManager::MouseButtonHandler, this);
}

void EditorManager::Start() {
	index = size = 0;
}

void EditorManager::DropEvent(Events::Event * event) {
	Events::DropInput* const input = static_cast<Events::DropInput*>(event);
	palette = Load::TMP(input->paths[0]);
	UpdateIndex();

	tilemap->palette = palette;
	tilemap->SetActive(false);
	tilemap->SetActive(true);
}

void EditorManager::KeyEvent(Events::Event * event) {
	if (size == 0) return;

	Events::KeyInput* const input = static_cast<Events::KeyInput*>(event);
	if (input->action == GLFW_PRESS) {
		if (input->key == GLFW_KEY_LEFT) {
			if (index == 0) {
				index = size - 1;
			} else {
				--index;
			}
			UpdateIndex();
		} else if (input->key == GLFW_KEY_RIGHT) {
			if (index == size - 1) {
				index = 0;
			} else {
				++index;
			}
			UpdateIndex();
		}
	}
}

void EditorManager::CursorPositionHandler(Events::Event * event) {
	Events::CursorPositionInput* const input = static_cast<Events::CursorPositionInput*>(event);
	cursorPosition = input->position;
}

void EditorManager::MouseButtonHandler(Events::Event * event) {
	if (!editorCamera->enabled) return;

	Events::MouseButtonInput* const input = static_cast<Events::MouseButtonInput*>(event);

	if (input->button == GLFW_MOUSE_BUTTON_LEFT && input->action == GLFW_PRESS) {
		while (tilemap->layout.grids.size() <= index)
			tilemap->layout.grids.push_back({});

		Transform* const camTransform = entities->GetComponent<Transform>(editorCamera->camera->entity);
		vec2f screenPositions = editorCamera->camera->ScreenToWorldPosition(cursorPosition);
		screenPositions += camTransform->GetWorldTranslation().xy;

		tilemap->layout.grids[index].push_back(Tile {
			cursorSelection,
			vec2f(
				roundf(screenPositions.x),
				roundf(screenPositions.y)
			)
		});

		tilemap->SetActive(false);
		tilemap->SetActive(true);
	}
}

void EditorManager::UpdateIndex() {
	TilemapTexture texture = palette.GetTexture(index);
	previewRender->SetSprite(texture.texture);

	const vec2u dimensions = texture.GetDimensions();
	const vec2f dims = vec2f(dimensions);
	previewTransform->SetScale(dims);
	sizeLabel->text = std::to_string(dimensions.x) + " x " + std::to_string(dimensions.y);
	grid->SetCellRect(0.f, 0.f, dims.x * 0.5f, dims.y * 0.5f);
}
