#include "MenuManager.h"

#include "ProjectDefines.h"
#include "InputEvents.h"
#include "CellScript.h"
#include "MainGame.h"

#include "EditorScene.h"
#include "StatsManager.h"

#include <Helpers/FileHelpers.h>
#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

#include <filesystem>

MenuManager::MenuManager()
	: selectionDelay(0.5f) {
}

//void MenuManager::AddMap(std::string const & filepath) {
//	maps.push_back(std::make_pair(filepath, Load::Map(filepath)));
//}

void MenuManager::SetSaveDir(std::string const & dir) {
	for (const auto& entry : std::filesystem::directory_iterator(dir)) {
		const auto path = entry.path();
		if (path.has_extension() && path.extension().string() == ".emd") {
			const std::string filepath = path.string();

			std::string name = path.filename().string();
			name = name.substr(0, name.find_last_of('-'));

			maps.push_back(std::make_pair(name, Load::Map(filepath)));
		}
	}
}

void MenuManager::NextSong() {
	++selected;
	if (selected > static_cast<int>(maps.size()))
		selected = 0;
	SwitchingSong();
}

void MenuManager::PreviousSong() {
	--selected;
	if (selected < 0)
		selected = maps.size();
	SwitchingSong();
}

unsigned MenuManager::NumberOfRows(TableViewScript * tableView) {
	return maps.size() + 1;
}

void MenuManager::CellForRow(TableViewScript * tableView, unsigned index, Transform * cell) {
	CellScript* const script = entities->GetComponent<CellScript>(cell->entity);
	script->index = index;	

	vec4f selectedColor;

	if (index == maps.size()) {
		script->button->handlers[MOUSE_CLICK].Bind(&MenuManager::OpenEditor, this, index);
		script->titleLabel->text = "OPEN EDITOR";
		selectedColor = COLOR_ORANGE;
	} else {
		script->button->handlers[MOUSE_CLICK].Bind(&MenuManager::SelectSong, this, index);
		script->titleLabel->text = maps[index].first;
		selectedColor = COLOR_RED;
	}

	vec4f color(0.f, 0.f, 0.f, 0.2f);
	if (index == selected) {
		color = selectedColor;
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
			vec4f selectedColor = index == maps.size() ? COLOR_ORANGE : COLOR_RED;
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
			if (maps.size() <= selected) {
				Transition(new EditorScene);
			} else {
				MainGame* scene = new MainGame;
				scene->mapPath = maps[selected].first;
				scene->mapData = maps[selected].second;
				Transition(scene);
			}
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

void MenuManager::ScrollHandler(Events::Event * event) {
	Events::ScrollInput* input = static_cast<Events::ScrollInput*>(event);
	scrollBt = scrollDelay;
	scrollOffset += input->data.y * scrollMultiplier;
	if (scrollOffset >= 1.f) {
		PreviousSong();
		scrollOffset = 0.f;
	} else if (scrollOffset <= -1.f) {
		NextSong();
		scrollOffset = 0.f;
	}
}

void MenuManager::OpenEditor(unsigned index) {
	if (selected == index) {
		Transition(new EditorScene);
	} else {
		selected = index;
		SwitchingSong();
	}
}

void MenuManager::SelectSong(unsigned index) {
	if (selected == index) {
		MainGame* scene = new MainGame;
		scene->mapPath = maps[selected].first;
		scene->mapData = maps[selected].second;
		Transition(scene);
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

	if (maps.size() <= selected) {
		bpmLabel->text = "";
		return;
	}

	const std::string path = maps[selected].first;
	MapData& map = maps[selected].second;
	bubble->Play(SongData{ map.audioPath, map.bpm });

	bpmLabel->text = "BPM: " + std::to_string(maps[selected].second.bpm);

	StatsManager* stats = StatsManager::Instance();

	if (!stats->HasHighscore(path)) 
		stats->InitHighscore(path);
	
	ScoreStats const& highscore = stats->GetHighscore(path);
	scoreLabel->text = "HIGH: "  + std::to_string(highscore.score);
	gradeLabel->text = highscore.grade;
}

void MenuManager::Transition(Scene* const destination) {
	bubble->FadeOut(Handler<void, void>([destination]() {
		EventsManager::Get()->Trigger("PRESENT_SCENE", new Events::PresentScene(destination));
	}));
}
