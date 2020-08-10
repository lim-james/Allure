#include "MenuScene.h"

#include "Camera.h"
#include "Text.h"
#include "SpriteRender.h"
#include "AnimationSystem.h"
#include "AudioSystem.h"
#include "LayoutSystem.h"
#include "ButtonSystem.h"
// scripts
#include "MenuCamera.h"
#include "BubbleManager.h"
#include "MenuManager.h"
#include "TableViewScript.h"
// post vfx
#include "Bloom.h"
// utilities
#include "Layers.h"
#include "LoadFNT.h"
#include "LoadTexture.h"
#include "ProjectDefines.h"

#include <Helpers./ColorHelpers.h>
#include <Events/EventsManager.h>

void MenuScene::Awake() {
	Scene::Awake();

	systems->Subscribe<AudioSystem>(0);
	systems->Subscribe<AnimationSystem>(1);
	systems->Subscribe<LayoutSystem>(1);
	systems->Subscribe<ButtonSystem>(1);

	spectrumBubble = new Material::SpectrumBubble;
	spectrumBubble->minRadius = 0.5f;
	spectrumBubble->maxValue = 1.f;
	spectrumBubble->unit = 1.f;
	//spectrumBubble->multiplier = 1.5f;
	spectrumBubble->multiplier = 10.f;

	circle = new Material::Circle;
	circle->borderWeight = 0.1f;

	audioPlayer = new AudioPlayer;
	audioPlayer->Initialize(entities);

	tableViewCell = new TableViewCell;
	tableViewCell->Initialize(entities);

	sfxEmitter = new SFXEmitter;
	sfxEmitter->Initialize(entities);
}

void MenuScene::Create() {
	Scene::Create();

	Font* const vcrMono = Load::FNT("Files/Fonts/vcr_ocd_mono.fnt", "Files/Fonts/vcr_ocd_mono.png");

	{
		Transform* const transform = entities->GetComponent<Transform>(mainCamera);
		transform->SetLocalTranslation(vec3f(0.f, 0.f, 20.f));

		Camera* const camera = entities->GetComponent<Camera>(mainCamera);
		camera->cullingMask = DEFAULT;
	}

	// ui camera
	Camera* uiCamera = nullptr;
	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetLocalTranslation(vec3f(0.f, 0.f, 10.f));

		uiCamera = entities->AddComponent<Camera>(entity);
		uiCamera->SetActive(true);
		uiCamera->SetSize(20.f);
		uiCamera->SetDepth(1);
		uiCamera->shouldClear = false;
		uiCamera->projection = ORTHOGRAPHIC;
		uiCamera->cullingMask = UI;

		MenuCamera* const control = entities->AddComponent<MenuCamera>(entity);
		control->SetActive(true);
		control->speed = 1.f;
		control->range = 2.f;
	}

	tableViewCell->uiCamera = uiCamera;
	tableViewCell->font = vcrMono;

	// post processing volume
	{
		const unsigned volume = entities->Create();
		Bloom* bloom = entities->AddComponent<Bloom>(volume);
		bloom->SetActive(true);
		bloom->unit = 2.f;
	}

	// grid
	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetLocalTranslation(vec3f(0.f, 0.f, -5.f));
		transform->SetScale(vec3f(160.0f, 90.0f, 1.0f));
		transform->SetDynamic(false);

		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);
		render->SetSprite(Load::TGA("Files/Textures/tile.tga"));
		render->SetCellRect(0, 0, 32, 18);
		render->tint.a = 0.15f;

		Animator* const animation = entities->AddComponent<Animator>(entity);
		animation->SetActive(true);
		animation->Queue(AnimationBase(false, 1.f, 2.f), &render->tint.a, 0.15f);
		animation->Queue(AnimationBase(true, 5.f), &render->uvRect.y, 1.f);
	}

	// spectrum bubble
	BubbleManager* bubble = nullptr;
	Transform* bubleTransform = nullptr;
	{
		const unsigned entity = entities->Create();
		entities->SetLayer(entity, UI);

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetLocalTranslation(vec3f(0.f, 0.f, -2.f));
		transform->SetScale(30.f);
		bubleTransform = transform;

		Layout* const layout = entities->AddComponent<Layout>(entity);
		layout->SetActive(true);
		layout->AddConstraint(Constraint{ CENTER_X_ANCHOR, nullptr, LEFT_ANCHOR, 1.f, 5.f, uiCamera });
		layout->AddConstraint(Constraint{ CENTER_Y_ANCHOR, nullptr, TOP_ANCHOR, 1.f, -10.f, uiCamera });
		layout->AddConstraint(Constraint{ HEIGHT, nullptr, HEIGHT, 4.f, 0.f, uiCamera });
		layout->AddConstraint(Constraint{ WIDTH, transform, HEIGHT, 1.f, 0.f, uiCamera });

		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);
		render->SetMaterial(spectrumBubble);
		render->tint.a = 0.f;
		//render->SetSprite(Load::Texture2D("Files/Textures/vinyl.png"));

		Animator* const animation = entities->AddComponent<Animator>(entity);
		animation->SetActive(true);
		animation->Queue(AnimationBase(false, 1.f, 1.f), &render->tint.a, 1.f);

		bubble = entities->AddComponent<BubbleManager>(entity);
		bubble->SetActive(true);
		bubble->material = spectrumBubble;
		bubble->audioPrefab = audioPlayer;
		bubble->startFrequency = 20;
		bubble->endFrequency = 2000;
		bubble->audioDuration = 90.f;
		bubble->minRadius = 0.5f;
		bubble->maxRadius = 0.55f;
		bubble->rotationSpeed = 20.f;
		bubble->multiplier = 10.f;
	}

	Transform* titleTransform = nullptr;
	{
		const unsigned entity = entities->Create();
		entities->SetLayer(entity, UI);

		titleTransform = entities->GetComponent<Transform>(entity);
		titleTransform->SetLocalTranslation(vec3f(0.f, 0.f, 5.f));

		Layout* const layout = entities->AddComponent<Layout>(entity);
		layout->SetActive(true);
		layout->AddConstraint(Constraint{ LEFT_ANCHOR, nullptr, LEFT_ANCHOR, 1.f, 2.f, uiCamera });
		layout->AddConstraint(Constraint{ TOP_ANCHOR, nullptr, TOP_ANCHOR, 1.f, -2.f, uiCamera });

		Text* const text = entities->AddComponent<Text>(entity);
		text->SetActive(true);
		text->SetFont(vcrMono);
		text->paragraphAlignment = PARAGRAPH_LEFT;
		text->verticalAlignment = ALIGN_TOP;
		text->scale = 3.f;
		text->text = "TEMPO";
		text->color = COLOR_RED;
	}

	Transform* gradeTransform = nullptr;
	Text* gradeLabel = nullptr;
	{
		const unsigned entity = entities->Create();
		entities->SetLayer(entity, UI);

		gradeTransform = entities->GetComponent<Transform>(entity);
		gradeTransform->SetLocalTranslation(vec3f(0.f, 0.f, 5.f));
		gradeTransform->SetScale(4.f);

		Layout* const layout = entities->AddComponent<Layout>(entity);
		layout->SetActive(true);
		layout->AddConstraint(Constraint{ LEFT_ANCHOR, nullptr, LEFT_ANCHOR, 1.f, 2.f, uiCamera });
		//layout->AddConstraint(Constraint{ TOP_ANCHOR, titleTransform, BOTTOM_ANCHOR, 1.f, -5.0f, uiCamera });
		layout->AddConstraint(Constraint{ CENTER_Y_ANCHOR, bubleTransform, CENTER_Y_ANCHOR, 1.f, 0.0f, uiCamera });

		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);
		render->SetMaterial(circle);
		render->tint = COLOR_RED;

		Text* const text = entities->AddComponent<Text>(entity);
		text->SetActive(true);
		text->SetFont(vcrMono);
		text->scale = 1.5f;
		text->text = "S+";
		gradeLabel = text;
	}

	Transform* songTransform = nullptr;
	Text* bpmLabel = nullptr;
	{
		const unsigned entity = entities->Create();
		entities->SetLayer(entity, UI);

		songTransform = entities->GetComponent<Transform>(entity);
		songTransform->SetLocalTranslation(vec3f(0.f, 0.f, 5.f));
		songTransform->SetScale(0.f);

		Layout* const layout = entities->AddComponent<Layout>(entity);
		layout->SetActive(true);
		layout->AddConstraint(Constraint{ LEFT_ANCHOR, gradeTransform, RIGHT_ANCHOR, 1.f, 1.f, uiCamera });
		layout->AddConstraint(Constraint{ CENTER_Y_ANCHOR, gradeTransform, CENTER_Y_ANCHOR, 1.f, 0.25f, uiCamera });

		bpmLabel = entities->AddComponent<Text>(entity);
		bpmLabel->SetActive(true);
		bpmLabel->SetFont(vcrMono);
		bpmLabel->paragraphAlignment = PARAGRAPH_LEFT;
		bpmLabel->verticalAlignment = ALIGN_BOTTOM;
		bpmLabel->text = "BPM: 30";
	}

	Transform* scoreTransform = nullptr;
	Text* scoreLabel = nullptr;
	{
		const unsigned entity = entities->Create();
		entities->SetLayer(entity, UI);

		scoreTransform = entities->GetComponent<Transform>(entity);
		scoreTransform->SetLocalTranslation(vec3f(0.f, 0.f, 5.f));
		scoreTransform->SetScale(0.f);

		Layout* const layout = entities->AddComponent<Layout>(entity);
		layout->SetActive(true);
		layout->AddConstraint(Constraint{ LEFT_ANCHOR, gradeTransform, RIGHT_ANCHOR, 1.f, 1.f, uiCamera });
		layout->AddConstraint(Constraint{ CENTER_Y_ANCHOR, gradeTransform, CENTER_Y_ANCHOR, 1.f, -0.22f, uiCamera });

		Text* const text = entities->AddComponent<Text>(entity);
		text->SetActive(true);
		text->SetFont(vcrMono);
		text->paragraphAlignment = PARAGRAPH_LEFT;
		text->verticalAlignment = ALIGN_TOP;
		text->text = "BEST: 9020";
		scoreLabel = text;
	}

	// menu manager
	{
		const unsigned entity = entities->Create();

		Transform * const transform = entities->GetComponent<Transform>(entity);
		transform->SetScale(vec3f(20.f, 1.f, 1.f));

		Layout* const layout = entities->AddComponent<Layout>(entity);
		layout->SetActive(true);
		layout->AddConstraint(Constraint{ RIGHT_ANCHOR, nullptr, RIGHT_ANCHOR, 1.f, -2.5f, uiCamera });

		MenuManager* const manager = entities->AddComponent<MenuManager>(entity);
		manager->SetActive(true);
		manager->sfxEmitter = sfxEmitter;
		manager->bpmLabel = bpmLabel;
		manager->scoreLabel = scoreLabel;
		manager->gradeLabel = gradeLabel;
		manager->bubble = bubble;
		manager->SetSaveDir("Files/Data/Maps/");
		//manager->AddSong(SongData{ "128C", "Files/Media/128C.wav", 128 });
		//manager->AddSong(SongData{ "BLACKPINK How You Like That", "Files/Media/HowYouLikeThat.wav", 132 });
		//manager->AddSong(SongData{ "LOUD - Thoughts", "Files/Media/LOUD - Thoughts.wav", 150 });
		//manager->AddSong(SongData{ "Ice Flow", "Files/Media/IceFlow.wav", 140 });
		//manager->AddSong(SongData{ "Ken Blast - The Top", "Files/Media/Ken Blast - The Top.wav", 160 });
		//manager->AddSong(SongData{ "Running in the 90's", "Files/Media/Running in the 90's.wav", 118 });

		// holder transform for tableview
		const unsigned holder = entities->Create();
		Transform * const holderTransform = entities->GetComponent<Transform>(holder);
		holderTransform->SetScale(transform->GetScale());
		holderTransform->SetParent(transform);

		TableViewScript* const tableView = entities->AddComponent<TableViewScript>(entity);
		tableView->SetActive(true);
		tableView->cellPrefab = tableViewCell;
		tableView->rowHeight = 3.f;
		tableView->rowSpacing = 0.5f;
		tableView->holder = holderTransform;
		tableView->scrollSpeed = 10.f;
		tableView->numberOfRows.Bind(&MenuManager::NumberOfRows, manager);
		tableView->cellForRow.Bind(&MenuManager::CellForRow, manager);
		tableView->updateRow.Bind(&MenuManager::UpdateRow, manager);
	}
}

void MenuScene::Destroy() {
	Scene::Destroy();

	delete spectrumBubble;
	delete audioPlayer;
	delete tableViewCell;
	delete sfxEmitter;
}
