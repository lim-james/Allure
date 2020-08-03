#include "EditorScene.h"

#include "LayoutSystem.h"
#include "Camera.h"
#include "Text.h"
#include "SpriteRender.h"
#include "TilemapRender.h"
#include "ButtonSystem.h"

#include "EditorCamera2D.h"
#include "EditorManager.h"

#include "Layers.h"
#include "LoadFNT.h"
#include "LoadTexture.h"

void EditorScene::Awake() {
	Scene::Awake();

	systems->Subscribe<LayoutSystem>(0);
	systems->Subscribe<ButtonSystem>(0);
}

void EditorScene::Create() {
	Scene::Create();

	Font* font = Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga");

	EditorCamera2D* editorCamera = nullptr;
	{
		Camera* const camera = entities->GetComponent<Camera>(mainCamera);
		camera->clearColor = vec4f(1.f);
		camera->projection = ORTHOGRAPHIC;
		camera->cullingMask -= UI;

		editorCamera = entities->AddComponent<EditorCamera2D>(mainCamera);
		editorCamera->SetActive(true);
		editorCamera->camera = camera;
	}

	// UI camera
	Camera* uiCamera = nullptr;;
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
	}
	
	// manager
	EditorManager* editorManager = nullptr;
	{
		const unsigned entity = entities->Create();

		editorManager = entities->AddComponent<EditorManager>(entity);
		editorManager->SetActive(true);
		editorManager->editorCamera = editorCamera;
		editorManager->uiCamera = uiCamera;
	}

	// layer label
	{
		const unsigned entity = entities->Create();
		entities->SetLayer(entity, UI);

		Transform* const transform = entities->GetComponent<Transform>(entity);

		Layout* const layout = entities->AddComponent<Layout>(entity);
		layout->SetActive(true);
		layout->AddConstraint(Constraint(TOP_ANCHOR, nullptr, TOP_ANCHOR, 1.f, -2.f, uiCamera));

		Text* const text = entities->AddComponent<Text>(entity);
		text->SetActive(true);
		text->SetFont(font);
		text->text = "MAIN LAYER";
		text->color = vec4f(0.f, 0.f, 0.f, 1.f);
	}

	// size label
	Transform* sizeLabelTransform = nullptr;
	{
		const unsigned entity = entities->Create();
		entities->SetLayer(entity, UI);

		sizeLabelTransform = entities->GetComponent<Transform>(entity);

		Layout* const layout = entities->AddComponent<Layout>(entity);
		layout->SetActive(true);
		layout->AddConstraint(Constraint(TOP_ANCHOR, nullptr, TOP_ANCHOR, 1.f, -2.f, uiCamera));
		layout->AddConstraint(Constraint(RIGHT_ANCHOR, nullptr, RIGHT_ANCHOR, 1.f, -2.f, uiCamera));

		Text* const text = entities->AddComponent<Text>(entity);
		text->SetActive(true);
		text->SetFont(font);
		text->paragraphAlignment = PARAGRAPH_RIGHT;
		text->text = "Drag palette in.";
		text->color = vec4f(0.f, 0.f, 0.f, 1.f);

		editorManager->sizeLabel = text;
	}

	// preview
	Transform* previewTransform = nullptr;
	{
		const unsigned entity = entities->Create();
		entities->SetLayer(entity, UI);

		previewTransform = entities->GetComponent<Transform>(entity);

		Layout* const layout = entities->AddComponent<Layout>(entity);
		layout->SetActive(true);
		layout->AddConstraint(Constraint(TOP_ANCHOR, sizeLabelTransform, BOTTOM_ANCHOR, 1.f, -1.f, uiCamera));
		layout->AddConstraint(Constraint(RIGHT_ANCHOR, nullptr, RIGHT_ANCHOR, 1.f, -2.f, uiCamera));

		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);

		editorManager->SetPreview(render);

		Button* const button = entities->AddComponent<Button>(entity);
		button->SetActive(true);
		button->handlers[MOUSE_OVER].Bind(&EditorManager::PreviewMouseOver, editorManager);
		button->handlers[MOUSE_OUT].Bind(&EditorManager::PreviewMouseOut, editorManager);
		button->handlers[MOUSE_CLICK].Bind(&EditorManager::PreviewClick, editorManager);
	}

	// preview back
	{
		const unsigned entity = entities->Create();
		entities->SetLayer(entity, UI);

		Transform* const transform = entities->AddComponent<Transform>(entity);
		transform->SetLocalTranslation(vec3f(0.f, 0.f, -1.f));

		Layout* const layout = entities->AddComponent<Layout>(entity);
		layout->SetActive(true);
		layout->AddConstraint(Constraint(CENTER_X_ANCHOR, previewTransform, CENTER_X_ANCHOR, 1.f, 0.f, uiCamera));
		layout->AddConstraint(Constraint(CENTER_Y_ANCHOR, previewTransform, CENTER_Y_ANCHOR, 1.f, 0.f, uiCamera));
		layout->AddConstraint(Constraint(WIDTH, previewTransform, WIDTH, 1.f, 0.5f, uiCamera));
		layout->AddConstraint(Constraint(HEIGHT, previewTransform, HEIGHT, 1.f, 0.5f, uiCamera));

		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);
		render->tint = vec4f(0.f, 0.f, 0.f, 1.f);
	}

	// grid
	{
		const unsigned entity = entities->Create();
		entities->SetLayer(entity, UI);

		Transform* const transform = entities->AddComponent<Transform>(entity);
		transform->SetLocalTranslation(vec3f(0.f, 0.f, 1.f));

		Layout* const layout = entities->AddComponent<Layout>(entity);
		layout->SetActive(true);
		layout->AddConstraint(Constraint(CENTER_X_ANCHOR, previewTransform, CENTER_X_ANCHOR, 1.f, 0.f, uiCamera));
		layout->AddConstraint(Constraint(CENTER_Y_ANCHOR, previewTransform, CENTER_Y_ANCHOR, 1.f, 0.f, uiCamera));
		layout->AddConstraint(Constraint(WIDTH, previewTransform, WIDTH, 1.f, 0.f, uiCamera));
		layout->AddConstraint(Constraint(HEIGHT, previewTransform, HEIGHT, 1.f, 0.f, uiCamera));

		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);
		render->SetSprite(Load::Texture2D("Files/Textures/grid.png"));

		editorManager->grid = render;
	}

	// cursor
	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		editorCamera->cursor = transform;

		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);
		render->tint = vec4f(0.f, 0.f, 0.f, 1.f);
	}

	// background
	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetLocalTranslation(vec3f(0.5f, 0.5f, -5.f));
		transform->SetScale(vec3f(100.f));

		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);
		render->SetSprite(Load::Texture2D("Files/Textures/grid.png"));
		render->SetCellRect(0.f, 0.f, 50.f, 50.f);
		render->tint = vec4f(0.f, 0.f, 0.f, 0.25f);
	}

	// tilemap
	{
		const unsigned entity = entities->Create();

		TilemapRender* const render = entities->AddComponent<TilemapRender>(entity);
		render->SetActive(true);

		editorManager->tilemap = render;
	}
}
