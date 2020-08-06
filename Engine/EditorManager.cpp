#include "EditorManager.h"

#include "InputEvents.h"
#include "LoadTilemap.h"

#include <Events/EventsManager.h>
#include <Helpers/FileHelpers.h>
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
	layerHeight = index = size = 0;
	layerLabel->text = "LAYER 0";
}

void EditorManager::DropEvent(Events::Event * event) {
	Events::DropInput* const input = static_cast<Events::DropInput*>(event);

	const std::string path = input->paths[0];
	const std::string ext = Helpers::GetFileExt(path);
	
	if (ext == "tmp") {
		palette = Load::TMP(path);
		size = palette.GetTextures().size();
		UpdateIndex();

		for (auto& pair : tilemaps) {
			pair.second->SetPalette(palette);
			pair.second->SetActive(false);
			pair.second->SetActive(true);
		}
	} else if (ext == "csv") {
		if (tilemaps[layerHeight] == nullptr)
				CreateLayer(layerHeight);
		TilemapRender* const tilemap = tilemaps[layerHeight];
		tilemap->layout = Load::TML(path);
	} else {
		Debug::Error << "File type not recognised.\n";
	}

}

void EditorManager::KeyEvent(Events::Event * event) {
	Events::KeyInput* const input = static_cast<Events::KeyInput*>(event);

	if (input->action == GLFW_PRESS) {
		if (input->key == GLFW_KEY_LEFT) {
			if (size == 0) return;
			if (index == 0) {
				index = size - 1;
			} else {
				--index;
			}
			UpdateIndex();
		} else if (input->key == GLFW_KEY_RIGHT) {
			if (size == 0) return;
			if (index == size - 1) {
				index = 0;
			} else {
				++index;
			}
			UpdateIndex();
		} else if (input->key == GLFW_KEY_UP) {
			++layerHeight;
			layerLabel->text = "LAYER " + std::to_string(layerHeight);

			Transform* const cursor = editorCamera->cursor;
			vec3f position = cursor->GetLocalTranslation();
			position.z = static_cast<float>(layerHeight);
			cursor->SetLocalTranslation(position);
		} else if (input->key == GLFW_KEY_DOWN) {
			--layerHeight;
			layerLabel->text = "LAYER " + std::to_string(layerHeight);

			Transform* const cursor = editorCamera->cursor;
			vec3f position = cursor->GetLocalTranslation();
			position.z = static_cast<float>(layerHeight);
			cursor->SetLocalTranslation(position);
		} else if (input->key == GLFW_KEY_ENTER) {
			const std::string prefix = "Files/Data/Levels/levels-(";
			const std::string sufix = ").csv";
			for (auto& tm : tilemaps) {
				Write::TML(tm.second->layout, prefix + std::to_string(tm.first) + sufix);
			}
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
		if (tilemaps[layerHeight] == nullptr)
			CreateLayer(layerHeight);
		TilemapRender* const tilemap = tilemaps[layerHeight];

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

void EditorManager::CreateLayer(int const & z) {
	const unsigned entity = entities->Create();

	Transform* const transform = entities->GetComponent<Transform>(entity);
	transform->SetLocalTranslation(vec3f(0.f, 0.f, static_cast<float>(z)));

	TilemapRender* const render = entities->AddComponent<TilemapRender>(entity);
	render->SetPalette(palette);
	render->SetActive(true);
	render->SetMaterial(tilemapLit);

	tilemaps[z] = render;
}
