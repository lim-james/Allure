#include "EditorScene.h"

#include "Camera.h"
#include "SpriteRender.h"
#include "Text.h"
#include "AudioSystem.h"
#include "LayoutSystem.h"
#include "AnimationSystem.h"
#include "ButtonSystem.h"
#include "SpriteAnimationSystem.h"
// scripts
#include "ScreenShake.h"
#include "EditorCamera2D.h"
#include "EditorManager.h"
#include "AVController.h"
#include "AVButtonHandler.h"
#include "ScheduleController.h"
// utilities
#include "ProjectDefines.h"
#include "LoadFNT.h"
#include "LoadTexture.h"
#include "Layers.h"

void EditorScene::Awake() {
	Scene::Awake();

	systems->Subscribe<AudioSystem>(0);
	systems->Subscribe<LayoutSystem>(1);
	systems->Subscribe<AnimationSystem>(1);
	systems->Subscribe<ButtonSystem>(1);
	systems->Subscribe<SpriteAnimationSystem>(2);

	circle = new Material::Circle;
	circle->borderWeight = 0.1f;

	avButton = new AVButton;
	avButton->Initialize(entities);
	avButton->material = circle;

	// guns

	pistol = new PistolPreview;
	pistol->Initialize(entities);

	automatic = new AutomaticPreview;
	automatic->Initialize(entities);

	shotgun = new ShotgunPreview;
	shotgun->Initialize(entities);

	sniper = new SniperPreview;
	sniper->Initialize(entities);

	grenadeLauncher = new GrenadeLauncherPreview;
	grenadeLauncher->Initialize(entities);

	laser = new LaserPreview;
	laser->Initialize(entities);

	// enemies

	basicPreview = new BasicPreview;
	basicPreview->Initialize(entities);

	batPreview = new BatPreview;
	batPreview->Initialize(entities);

	fireElementalPreview = new FireElementalPreview;
	fireElementalPreview->Initialize(entities);

	iceElementalPreview = new IceElementalPreview;
	iceElementalPreview->Initialize(entities);

	cyclopsPreview = new CyclopsPreview;
	cyclopsPreview->Initialize(entities);

}

void EditorScene::Create() {
	Scene::Create();

	Font* const vcrMono = Load::FNT("Files/Fonts/vcr_ocd_mono.fnt", "Files/Fonts/vcr_ocd_mono.png");

	Camera* camera = nullptr;
	{
		camera = entities->GetComponent<Camera>(mainCamera);
		camera->SetSize(30.f);
		camera->projection = ORTHOGRAPHIC;
		camera->clearColor = vec4f(0.f);
		camera->cullingMask = DEFAULT;

		AudioListener* const listener = entities->AddComponent<AudioListener>(mainCamera);
		listener->SetActive(true);

		ScreenShake* const shake = entities->AddComponent<ScreenShake>(mainCamera);
		shake->SetActive(true);
		shake->duration = 0.25f;
		shake->magnitude = 0.25f;

		EditorCamera2D* const editorCamera = entities->AddComponent<EditorCamera2D>(mainCamera);
		editorCamera->SetActive(true);
		editorCamera->minZoom = 10.f;
		editorCamera->maxZoom = 60.f;
	}

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
		uiCamera->SetUseProcess(false);
	}

	EditorManager* editorManager = nullptr;
	AVController* avController = nullptr;
	ScheduleController* scheduleController = nullptr;
	{
		const unsigned entity = entities->Create();

		Animator* const animator = entities->AddComponent<Animator>(entity);
		animator->SetActive(true);

		TransformAnimator* const tAnimator = entities->AddComponent<TransformAnimator>(entity);
		tAnimator->SetActive(true);

		AudioSource* const audio = entities->AddComponent<AudioSource>(entity);
		audio->SetActive(true);
		audio->loop = true;

		editorManager = entities->AddComponent<EditorManager>(entity);
		editorManager->SetActive(true);
		editorManager->animator = animator;
		editorManager->tAnimator = tAnimator;

		avController = entities->AddComponent<AVController>(entity);
		avController->SetActive(true);
		avController->source = audio;

		editorManager->controller = avController;	

		scheduleController = entities->AddComponent<ScheduleController>(entity);
		scheduleController->SetActive(true);
		scheduleController->avController = avController;
		scheduleController->mainCamera = camera;
		scheduleController->animator = animator;
		scheduleController->AddEnemy(EnemyPreviewData{ basicPreview, shotgun });
		scheduleController->AddEnemy(EnemyPreviewData{ basicPreview, pistol });
		scheduleController->AddEnemy(EnemyPreviewData{ fireElementalPreview, nullptr });
		scheduleController->AddEnemy(EnemyPreviewData{ cyclopsPreview, nullptr });

		avController->indexChangeHandler.Bind(&ScheduleController::IndexChangeHandler, scheduleController);
	}

	{
		const unsigned entity = entities->Create();
		entities->SetLayer(entity, UI);

		Text* const text = entities->AddComponent<Text>(entity);
		text->SetActive(true);
		text->SetFont(vcrMono);
		text->text = "DRAG IN YOUR WAV FILE.";

		editorManager->promptLabel = text;
	}

	// background
	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetLocalTranslation(vec3f(0.f, 0.f, -5.f));
		transform->SetScale(vec3f(160.0f, 90.0f, 1.0f));
		transform->SetDynamic(false);

		scheduleController->backgroundTransform = transform;

		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);
		render->SetSprite(Load::TGA("Files/Textures/tile.tga"));
		render->SetCellRect(0, 0, 12, 6.75);
		render->tint.a = 0.15f;

		editorManager->background = render;

		Button* const button = entities->AddComponent<Button>(entity);
		button->SetActive(true);
		button->handlers[MOUSE_CLICK].Bind(&ScheduleController::OnCanvasClick, scheduleController);
	}

	// av holder
	Transform* avHolder = nullptr;
	{
		const unsigned entity = entities->Create();
		entities->SetLayer(entity, UI);

		avHolder = entities->GetComponent<Transform>(entity);

		Layout* const layout = entities->AddComponent<Layout>(entity);
		layout->SetActive(true);
		layout->AddConstraint(Constraint(HEIGHT, nullptr, NA, 1.f, 6.f, uiCamera));
		layout->AddConstraint(Constraint(WIDTH, nullptr, WIDTH, 1.f, 0.f, uiCamera));
		layout->AddConstraint(Constraint(BOTTOM_ANCHOR, nullptr, BOTTOM_ANCHOR, 1.f, -6.f, uiCamera));

		editorManager->avLayout = layout;
	}

	// title label
	{
		const unsigned entity = entities->Create();
		entities->SetLayer(entity, UI);

		Layout* const layout = entities->AddComponent<Layout>(entity);
		layout->SetActive(true);
		layout->AddConstraint(Constraint(LEFT_ANCHOR, avHolder, LEFT_ANCHOR, 1.f, 2.f, uiCamera));
		layout->AddConstraint(Constraint(CENTER_Y_ANCHOR, avHolder, CENTER_Y_ANCHOR, 1.f, 0.f, uiCamera));

		Text* const text = entities->AddComponent<Text>(entity);
		text->SetActive(true);
		text->SetFont(vcrMono);
		text->paragraphAlignment = PARAGRAPH_LEFT;

		avController->titleLabel = text;
	}

	// BPM label
	{
		const unsigned entity = entities->Create();
		entities->SetLayer(entity, UI);

		Layout* const layout = entities->AddComponent<Layout>(entity);
		layout->SetActive(true);
		layout->AddConstraint(Constraint(RIGHT_ANCHOR, avHolder, RIGHT_ANCHOR, 1.f, -2.f, uiCamera));
		layout->AddConstraint(Constraint(CENTER_Y_ANCHOR, avHolder, CENTER_Y_ANCHOR, 1.f, 0.f, uiCamera));
		layout->AddConstraint(Constraint(WIDTH, nullptr, NA, 1.f, 5.f, uiCamera));

		Text* const text = entities->AddComponent<Text>(entity);
		text->SetActive(true);
		text->SetFont(vcrMono);
		text->paragraphAlignment = PARAGRAPH_RIGHT;
		text->text = "60 BPM";

		avController->bpmLabel = text;

		Button* const button = entities->AddComponent<Button>(entity);
		button->SetActive(true);
		button->handlers[MOUSE_CLICK].Bind(&AVController::OnClickBPM, avController);
	}

	// time label
	Transform* timeTransform = nullptr;
	{
		const unsigned entity = entities->Create();
		entities->SetLayer(entity, UI);

		timeTransform = entities->GetComponent<Transform>(entity);

		Layout* const layout = entities->AddComponent<Layout>(entity);
		layout->SetActive(true);
		layout->AddConstraint(Constraint(CENTER_Y_ANCHOR, avHolder, TOP_ANCHOR, 1.f, -0.5f, uiCamera));
		layout->AddConstraint(Constraint(LEFT_ANCHOR, avHolder, LEFT_ANCHOR, 1.f, 2.f, uiCamera));
		layout->AddConstraint(Constraint(WIDTH, nullptr, NA, 1.f, 3.f, uiCamera));

		Text* const text = entities->AddComponent<Text>(entity);
		text->SetActive(true);
		text->SetFont(vcrMono);
		text->scale = 0.75f;
		text->paragraphAlignment = PARAGRAPH_LEFT;
		text->text = "00:00";

		avController->timeLabel = text;
	}

	// duration label
	Transform* durationTransform = nullptr;
	{
		const unsigned entity = entities->Create();
		entities->SetLayer(entity, UI);

		durationTransform = entities->GetComponent<Transform>(entity);

		Layout* const layout = entities->AddComponent<Layout>(entity);
		layout->SetActive(true);
		layout->AddConstraint(Constraint(CENTER_Y_ANCHOR, avHolder, TOP_ANCHOR, 1.f, -0.5f, uiCamera));
		layout->AddConstraint(Constraint(RIGHT_ANCHOR, avHolder, RIGHT_ANCHOR, 1.f, -2.f, uiCamera));
		layout->AddConstraint(Constraint(WIDTH, nullptr, NA, 1.f, 3.f, uiCamera));

		Text* const text = entities->AddComponent<Text>(entity);
		text->SetActive(true);
		text->SetFont(vcrMono);
		text->scale = 0.75f;
		text->paragraphAlignment = PARAGRAPH_RIGHT;
		text->text = "00:00";

		avController->durationLabel = text;
	}

	// Progress bar
	Transform* progressBar = nullptr;
	{
		const unsigned entity = entities->Create();
		entities->SetLayer(entity, UI);

		progressBar = entities->GetComponent<Transform>(entity);

		Layout* const layout = entities->AddComponent<Layout>(entity);
		layout->SetActive(true);
		layout->AddConstraint(Constraint(CENTER_Y_ANCHOR, avHolder, TOP_ANCHOR, 1.f, -0.5f, uiCamera));
		layout->AddConstraint(Constraint(LEFT_ANCHOR, timeTransform, RIGHT_ANCHOR, 1.f, 1.f, uiCamera));
		layout->AddConstraint(Constraint(RIGHT_ANCHOR, durationTransform, LEFT_ANCHOR, 1.f, -1.f, uiCamera));
		layout->AddConstraint(Constraint(HEIGHT, nullptr, NA, 1.f, 0.1f, uiCamera));

		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);
		render->tint.a = 0.25f;
	}

	// Progress indicator
	Transform* progressIndicator = nullptr;
	{
		const unsigned entity = entities->Create();
		entities->SetLayer(entity, UI);

		progressIndicator = entities->GetComponent<Transform>(entity);
		progressIndicator->SetLocalTranslation(vec3f(0.f, 0.f, 1.f));

		Layout* const layout = entities->AddComponent<Layout>(entity);
		layout->SetActive(true);
		layout->AddConstraint(Constraint(LEFT_ANCHOR, progressBar, LEFT_ANCHOR, 1.f, 0.f, uiCamera));
		layout->AddConstraint(Constraint(CENTER_Y_ANCHOR, progressBar, CENTER_Y_ANCHOR, 1.f, 0.f, uiCamera));
		layout->AddConstraint(Constraint(WIDTH, progressBar, WIDTH, 0.f, 0.f, uiCamera));
		layout->AddConstraint(Constraint(HEIGHT, progressBar, HEIGHT, 1.f, 0.f, uiCamera));

		avController->progressIndicator = layout;

		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);
	}

	// Beat label
	{
		const unsigned entity = entities->Create();
		entities->SetLayer(entity, UI);

		Layout* const layout = entities->AddComponent<Layout>(entity);
		layout->SetActive(true);
		layout->AddConstraint(Constraint(BOTTOM_ANCHOR, progressIndicator, TOP_ANCHOR, 1.f, 1.f, uiCamera));
		layout->AddConstraint(Constraint(CENTER_X_ANCHOR, progressIndicator, RIGHT_ANCHOR, 1.f, 0.f, uiCamera));

		Text* const text = entities->AddComponent<Text>(entity);
		text->SetActive(true);
		text->SetFont(vcrMono);
		text->verticalAlignment = ALIGN_BOTTOM;

		avController->beatLabel = text;
	}

	// center
	Transform* playTransform = nullptr;
	{
		playTransform = avButton->Create();
		const unsigned entity = playTransform->entity;

		Layout* const layout = entities->GetComponent<Layout>(entity);
		layout->AddConstraint(Constraint(CENTER_X_ANCHOR, avHolder, CENTER_X_ANCHOR, 1.f, 0.f, uiCamera));
		layout->AddConstraint(Constraint(CENTER_Y_ANCHOR, avHolder, CENTER_Y_ANCHOR, 1.f, 0.f, uiCamera));

		SpriteRender* const render = entities->GetComponent<SpriteRender>(entity);
		render->SetSprite(Load::Texture2D("Files/Textures/play.png"));

		avController->playButton = render;

		Button* const button = entities->GetComponent<Button>(entity);
		button->handlers[MOUSE_CLICK].Bind(&AVController::ToggleAudio, avController);

		AVButtonHandler* const avHandler = entities->GetComponent<AVButtonHandler>(entity);
		avHandler->animator = editorManager->animator;
		avHandler->tAnimator = editorManager->tAnimator;
	}

	// left
	{
		const unsigned entity = avButton->Create()->entity;

		Layout* const layout = entities->GetComponent<Layout>(entity);
		layout->AddConstraint(Constraint(CENTER_Y_ANCHOR, playTransform, CENTER_Y_ANCHOR, 1.f, 0.f, uiCamera));
		layout->AddConstraint(Constraint(RIGHT_ANCHOR, playTransform, LEFT_ANCHOR, 1.f, -2.f, uiCamera));

		SpriteRender* const render = entities->GetComponent<SpriteRender>(entity);
		render->SetSprite(Load::Texture2D("Files/Textures/previous.png"));

		Button* const button = entities->GetComponent<Button>(entity);
		button->handlers[MOUSE_CLICK].Bind(&AVController::PreviousBeat, avController);

		AVButtonHandler* const avHandler = entities->GetComponent<AVButtonHandler>(entity);
		avHandler->SetActive(true);
		avHandler->animator = editorManager->animator;
		avHandler->tAnimator = editorManager->tAnimator;
	}

	// right
	{
		const unsigned entity = avButton->Create()->entity;

		Layout* const layout = entities->GetComponent<Layout>(entity);
		layout->AddConstraint(Constraint(CENTER_Y_ANCHOR, playTransform, CENTER_Y_ANCHOR, 1.f, 0.f, uiCamera));
		layout->AddConstraint(Constraint(LEFT_ANCHOR, playTransform, RIGHT_ANCHOR, 1.f, 2.f, uiCamera));

		SpriteRender* const render = entities->GetComponent<SpriteRender>(entity);
		render->SetSprite(Load::Texture2D("Files/Textures/next.png"));

		Button* const button = entities->GetComponent<Button>(entity);
		button->handlers[MOUSE_CLICK].Bind(&AVController::NextBeat, avController);

		AVButtonHandler* const avHandler = entities->GetComponent<AVButtonHandler>(entity);
		avHandler->SetActive(true);
		avHandler->animator = editorManager->animator;
		avHandler->tAnimator = editorManager->tAnimator;
	}

	// belt holder
	Transform* beltTransform = nullptr;
	{
		const unsigned entity = entities->Create();
		entities->SetLayer(entity, UI);

		beltTransform = entities->GetComponent<Transform>(entity);
		scheduleController->beltTransform = beltTransform;

		Layout* const layout = entities->AddComponent<Layout>(entity);
		layout->SetActive(true);
		layout->AddConstraint(Constraint(CENTER_Y_ANCHOR, nullptr, TOP_ANCHOR, 1.f, 6.f, uiCamera));

		editorManager->beltLayout = layout;
	}
}

void EditorScene::Destroy() {
	Scene::Destroy();

	delete circle;

	delete avButton;

	delete pistol;
	delete automatic;
	delete shotgun;
	delete sniper;
	delete grenadeLauncher;
	delete laser;

	delete basicPreview;
	delete batPreview;
	delete fireElementalPreview;
	delete iceElementalPreview;
	delete cyclopsPreview;
}
