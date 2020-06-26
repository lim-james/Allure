#include "TableViewScript.h"

#include "InputEvents.h"

#include <Events/EventsManager.h>

void TableViewScript::ReloadData() {
	const float count = static_cast<float>(numberOfRows.InvokeReturn(this)[0]);
	const float halfH = rowHeight * 0.5f;

	vec3f cellSize = holder->GetScale();
	cellSize.y = rowHeight;

	const float stride = -GetRowStride();

	for (float i = 0; i < count; ++i) {
		Transform* const cTransform = cellPrefab->CreateIn(holder);
		cTransform->SetLocalTranslation(vec3f(0.f, scrollOffset + i * stride, 0.f));
		cTransform->SetScale(cellSize);

		cellForRow.Invoke(this, static_cast<unsigned>(i), cTransform);
		rows.push_back(cTransform);
	}

	contentHeight = -(count - 1) * stride;
}

void TableViewScript::UpdateRows() {
	for (unsigned i = 0; i < rows.size(); ++i)
		updateRow.Invoke(this, i, rows[i]);
}

float TableViewScript::GetRowStride() const {
	return rowHeight + rowSpacing;
}

void TableViewScript::Awake() {
	EventsManager::Get()->Subscribe("CURSOR_POSITION_INPUT", &TableViewScript::CursorPositionHandler, this);
	EventsManager::Get()->Subscribe("MOUSE_BUTTON_INPUT", &TableViewScript::MouseButtonHandler, this);
}

void TableViewScript::Start() {
	scrollOffset = 0.f;
	ReloadData();
}

void TableViewScript::Update() {
	vec3f translation = holder->GetLocalTranslation();
	translation.y = Math::Lerp(translation.y, scrollOffset, time->dt * scrollSpeed);
	translation.y = Math::Clamp(translation.y, 0.f, contentHeight);
	holder->SetLocalTranslation(translation);
}

void TableViewScript::CursorPositionHandler(Events::Event * event) {
	Events::CursorPositionInput* input = static_cast<Events::CursorPositionInput*>(event);
}

void TableViewScript::MouseButtonHandler(Events::Event * event) {
	Events::MouseButtonInput* input = static_cast<Events::MouseButtonInput*>(event);
}
