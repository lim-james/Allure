#include "MenuManager.h"

#include "ProjectDefines.h"
#include "InputEvents.h"
#include "CellScript.h"
#include "MainGame.h"
#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

MenuManager::MenuManager()
	: selectionDelay(0.5f) {
}

void MenuManager::AddSong(SongData const & data) {
	songs.push_back(data);
}

void MenuManager::NextSong() {
	++selected;
	if (selected >= static_cast<int>(songs.size()))
		selected = 0;
	SwitchingSong();
}

void MenuManager::PreviousSong() {
	--selected;
	if (selected < 0)
		selected = songs.size() - 1;
	SwitchingSong();
}

unsigned MenuManager::NumberOfRows(TableViewScript * tableView) {
	return songs.size();
}

void MenuManager::CellForRow(TableViewScript * tableView, unsigned index, Transform * cell) {
	CellScript* const script = entities->GetComponent<CellScript>(cell->entity);
	script->index = index;	

	script->button->handlers[MOUSE_CLICK].Bind(&MenuManager::SelectSong, this, index);

	script->titleLabel->text = songs[index].title;

	vec4f color(0.f, 0.f, 0.f, 0.2f);
	if (index == selected) {
		color = COLOR_RED;
		color.a = 0.8f;

		vec3f translation = cell->GetLocalTranslation();
		translation.x = -0.05f;
		cell->SetLocalTranslation(translation);
	}
	
	script->background->tint = color;
}

void MenuManager::UpdateRow(TableViewScript * tableView, unsigned index, Transform * cell) {
	CellScript* const script = entities->GetComponent<CellScript>(cell->entity);

	Animator* const animator = script->animator;
	SpriteRender* const background = script->background;

	if (index == selected) {
		if (background->tint.a != 0.8f) {
			vec4f selectedColor = COLOR_RED;
			selectedColor.a = 0.8f;
			
			animator->Clear(&background->tint);
			animator->Queue(
				AnimationBase(false, 0.25f),
				&background->tint,
				selectedColor
			);
	
			vec3f translation = cell->GetLocalTranslation();
			translation.x = -0.05f;
			cell->SetLocalTranslation(translation);
		}
	} else {
		if (background->tint.a != 0.2f) {
			animator->Clear(&background->tint);
			animator->Queue(
				AnimationBase(false, 0.25f),
				&background->tint,
				vec4f(0.f, 0.f, 0.f, 0.2f)
			);
	
			vec3f translation = cell->GetLocalTranslation();
			translation.x = 0.f;
			cell->SetLocalTranslation(translation);
		}
	}

}

void MenuManager::Awake() {
	scrollDelay = 0.5f;
	scrollMultiplier = 1.f;

	EventsManager::Get()->Subscribe("KEY_INPUT", &MenuManager::KeyHandler, this);
	EventsManager::Get()->Subscribe("SCROLL_INPUT", &MenuManager::ScrollHandler, this);
}

void MenuManager::Start() {
	switched = true;
	bt = 0.f;
	selected = 0;

	scrollBt = 0.f;

	tableView = GetComponent<TableViewScript>();
	tableView->UpdateRows();
}

void MenuManager::Update() {
	if (switched) {
		bt += time->dt;
		if (bt >= selectionDelay)
			UpdateSong();
	}

	if (scrollBt > 0.f) {
		scrollBt -= time->dt;
		if (scrollBt <= 0.f)
			scrollOffset = 0.f;
	}
}

void MenuManager::KeyHandler(Events::Event * event) {
	Events::KeyInput* input = static_cast<Events::KeyInput*>(event);

	if (input->action == GLFW_PRESS) {
		if (input->key == GLFW_KEY_ENTER) {
			Transition();
		}
	}

	if (input->action != GLFW_RELEASE) {
		if (input->key == GLFW_KEY_UP || input->key == GLFW_KEY_W) {
			PreviousSong();
		} else if (input->key == GLFW_KEY_DOWN || input->key == GLFW_KEY_S) {
			NextSong();
		}
	}
}

void MenuManager::SelectSong(unsigned index) {
	if (selected == index) {
		Transition();
	} else {
		selected = index;
		SwitchingSong();
	}
}

void MenuManager::SwitchingSong() {
	switched = true;
	bt = 0.f;
	bubble->FadeOut();

	tableView->scrollOffset = selected * tableView->GetRowStride();
	tableView->UpdateRows();
}

void MenuManager::UpdateSong() {
	switched = false;
	bt = 0.f;
	bubble->Play(songs[selected]);
}

void MenuManager::Transition() {
	SongData song = songs[selected];
	bubble->FadeOut(Handler<void, void>([song]() {
		MainGame* destination = new MainGame;
		destination->song = song;
		EventsManager::Get()->Trigger("PRESENT_SCENE", new Events::PresentScene(destination));
	}));
}

void MenuManager::ScrollHandler(Events::Event * event) {
	Events::ScrollInput* input = static_cast<Events::ScrollInput*>(event);
	scrollBt = scrollDelay;
	scrollOffset += input->data.y * scrollMultiplier;
	if (scrollOffset > 1.f) {
		PreviousSong();
		scrollOffset = 0.f;
	} else if (scrollOffset < -1.f) {
		NextSong();
		scrollOffset = 0.f;
	}
}
