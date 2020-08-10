#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include "Script.h"
#include "Text.h"
#include "BubbleManager.h"
#include "TableViewScript.h"
#include "MapData.h"
#include "Scene.h"

#include "TableViewScript.h"

#include <Handler/Handler.h>

struct MenuManager : Script {

	using base_type = MenuManager;

	Prefab* sfxEmitter;
	
	Text* bpmLabel;
	Text* scoreLabel;
	Text* gradeLabel;
	BubbleManager* bubble;

	Handler<void, std::string> rowChangeHandler;
	
	MenuManager();

	void SetSaveDir(std::string const& dir);
	void NextSong();
	void PreviousSong();

	// tableView methods
	unsigned NumberOfRows(TableViewScript* tableView);
	// mutate for row
	void CellForRow(TableViewScript* tableView, unsigned index, Transform* cell);
	// update row
	void UpdateRow(TableViewScript* tableView, unsigned index, Transform* cell);

private:

	TableViewScript* tableView;
	
	bool switched;
	float selectionDelay;
	float bt;
	int selected;

	std::string saveDir;
	std::vector<std::pair<std::string, MapData>> maps;

	float scrollBt, scrollDelay;
	float scrollOffset, scrollMultiplier;

	void Awake() override;
	void Start() override;
	void Update() override;

	void KeyHandler(Events::Event* event);
	void ScrollHandler(Events::Event* event);

	void OpenEditor(unsigned index);
	void SelectSong(unsigned index);
	void SwitchingSong();
	void UpdateSong();
	void Transition(Scene* const destination);

};

#endif
