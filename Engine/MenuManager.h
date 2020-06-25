#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include "Script.h"
#include "BubbleManager.h"
#include "SongData.h"

struct MenuManager : Script {

	using base_type = MenuManager;
	
	BubbleManager* bubble;
	
	MenuManager();

	void AddSong(SongData const& data);
	void NextSong();
	void PreviousSong();

private:

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


};

#endif
