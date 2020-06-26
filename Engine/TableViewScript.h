#ifndef TABLE_VIEW_SCRIPT_H
#define TABLE_VIEW_SCRIPT_H

#include "Script.h"
#include "Prefab.h"

#include <Events/Event.h>
#include <Handler/Handler.h>
#include <vector>

struct TableViewScript : Script {

	using base_type = TableViewScript;

	// cell attributes

	Prefab* cellPrefab;
	float rowHeight;
	float rowSpacing;

	// table view methods

	Transform* holder;
	float scrollSpeed;
	float scrollOffset;

	// returns row count
	Handler<unsigned, TableViewScript*> numberOfRows;
	// mutate for row
	Handler<void, TableViewScript*, unsigned, Transform*> cellForRow;
	// update row
	Handler<void, TableViewScript*, unsigned, Transform*> updateRow;

	// refresh data
	void ReloadData();
	// update visually
	void UpdateRows();

	float GetRowStride() const;

private:

	float contentHeight;

	std::vector<Transform*> rows;

	void Awake() override;
	void Start() override;
	void Update() override;

	void CursorPositionHandler(Events::Event* event);
	void MouseButtonHandler(Events::Event* event);

};

#endif
