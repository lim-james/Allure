#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include "Script.h"
#include "BubbleManager.h"
#include "TableViewScript.h"
#include "SongData.h"

#include "TableViewScript.h"

struct MenuManager : Script {

	using base_type = MenuManager;
	
	BubbleManager* bubble;
	
	MenuManager();

	void AddSong(SongData const& data);
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
	std::vector<SongData> songs;

	void Awake() override;
	void Start() override;
	void Update() override;

	void KeyHandler(Events::Event* event);

	void SwitchingSong();
	void UpdateSong();

	void ScrollHandler(Events::Event* event);

};

#endif
