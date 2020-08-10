#include "ScoreScene.h"

#include "SpriteRender.h"
#include "Vignette.h"
#include "Text.h"
#include "ProjectDefines.h"
#include "AnimationSystem.h"
#include "LayoutSystem.h"
#include "ButtonSystem.h"
#include "MenuCamera.h"
#include "ScoreManager.h"
// utils
#include "LoadFNT.h"
// destination
#include "MenuScene.h"

#include <Events/EventsManager.h>

void ScoreScene::Awake() {
	Scene::Awake();

	systems->Subscribe<AnimationSystem>(1);
	systems->Subscribe<LayoutSystem>(1);
	systems->Subscribe<ButtonSystem>(1);

	circle = new Material::Circle;
	circle->borderWeight = 0.1f;
}

void ScoreScene::Create() {
	Scene::Create();

	Font* const vcrMono = Load::FNT("Files/Fonts/vcr_ocd_mono.fnt", "Files/Fonts/vcr_ocd_mono.png");

	Camera* const camera = entities->GetComponent<Camera>(mainCamera);
	camera->SetSize(20.f);
	camera->projection = ORTHOGRAPHIC;

	{
		MenuCamera* const control = entities->AddComponent<MenuCamera>(mainCamera);
		control->SetActive(true);
		control->speed = 1.f;
		control->range = 1.f;
	}

	ScoreManager* manager = nullptr;
	{
		const unsigned entity = entities->Create();
	
		manager = entities->AddComponent<ScoreManager>(entity);
		manager->SetActive(true);
		manager->mapPath = mapPath;
		manager->data = data;
	}

	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetScale(vec3f(16.f, 9.f, 1.f));

		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		//render->SetActive(true);
		render->SetSprite(sceneTexture);

		Layout* const layout = entities->AddComponent<Layout>(entity);
		layout->SetActive(true);
		layout->AddConstraint(Constraint(WIDTH, nullptr, WIDTH, 1.f, 0.f, camera));
		layout->AddConstraint(Constraint(HEIGHT, nullptr, HEIGHT, 1.f, 0.f, camera));
	}

	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetLocalTranslation(vec3f(0.f, 7.f, 0.f));

		Text* const text = entities->AddComponent<Text>(entity);
		text->SetActive(true);
		text->SetFont(vcrMono);
		text->text = "NEW HIGHSCORE";
		text->color = COLOR_YELLOW;
		text->color.a = 0.f;
		text->offset.y = -1.f;
		text->scale = 0;

		Animator* const animation = entities->AddComponent<Animator>(entity);
		//animation->SetActive(true);
		animation->Queue(AnimationBase(false, 0.25f, 5.f), &text->offset.y, 0.f);
		animation->Queue(AnimationBase(false, 0.25f, 5.f), &text->color.a, 1.f);
		animation->Queue(AnimationBase(false, 0.25f, 5.f), &text->scale, 4.f);

		manager->highscore = animation;
	}

	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetLocalTranslation(vec3f(0.f, 2.f, 0.f));

		Text* const text = entities->AddComponent<Text>(entity);
		text->SetActive(true);
		text->SetFont(vcrMono);
		text->text = "GAME OVER.";
		text->color = COLOR_RED;
		text->color.a = 0.f;
		text->offset.y = -1.f;

		Animator* const animation = entities->AddComponent<Animator>(entity);
		animation->SetActive(true);
		animation->Queue(AnimationBase(false, 0.25f, 1.f), &text->offset.y, 0.f);
		animation->Queue(AnimationBase(false, 0.25f, 1.f), &text->color.a, 1.f);
	}

	CreateRow(0.f, "GOOD", std::to_string(data.good), vcrMono);
	CreateRow(1.f, "GREAT", std::to_string(data.great), vcrMono);
	CreateRow(2.f, "PERFECT", std::to_string(data.perfect), vcrMono);
	CreateRow(3.f, "MISSED", std::to_string(data.missed), vcrMono);
	CreateRow(4.f, "SCORE", std::to_string(data.score), vcrMono);

	const int completion = static_cast<int>(roundf((data.lifetime / data.duration) * 100.f));
	CreateRow(5.f, "COMPLETION", std::to_string(completion) + "%", vcrMono);

	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetLocalTranslation(vec3f(11.f, -2.5f, 0.f));
		transform->SetLocalRotation(vec3f(0.f, 0.f, 30.f));
		transform->SetScale(0.f);

		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);
		render->SetMaterial(circle);
		render->tint = COLOR_RED;
		
		Text* const text = entities->AddComponent<Text>(entity);
		text->SetActive(true);
		text->SetFont(vcrMono);
		text->scale = 0.f;
		text->color = WHITE;	

		manager->gradeLabel = text;

		TransformAnimator* const tAnimator = entities->AddComponent<TransformAnimator>(entity);
		tAnimator->SetActive(true);
		tAnimator->Queue(AnimationBase(false, 0.1f, 4.5f), transform, ANIMATE_SCALE, vec3f(7.5f, 7.5f, 1.f));

		Animator* const animator = entities->AddComponent<Animator>(entity);
		animator->SetActive(true);
		animator->Queue(AnimationBase(false, 0.1f, 4.5f), &text->scale, 4.f);
	}

	// retry button
	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetLocalTranslation(vec3f(0.f, -10.5f, 0.f));
		transform->SetScale(vec3f(7.f, 2.f, 0.f));

		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);
		render->tint = COLOR_RED;
		render->tint.a = 0.f;

		Text* const text = entities->AddComponent<Text>(entity);
		text->SetActive(true);
		text->SetFont(vcrMono);
		text->text = "RETRY";
		text->color = vec4f(1.f, 1.f, 1.f, 0.f);

		Button* const button = entities->AddComponent<Button>(entity);
		button->SetActive(true);
		button->handlers[MOUSE_CLICK].Bind([]() {
			EventsManager::Get()->Trigger("PRESENT_SCENE", new Events::PresentScene(new MenuScene));
		});
		//button->handlers[MOUSE_OVER].Bind([transform]() {
		//	transform->Parse(transform->ToString());
		//});

		Animator* const animation = entities->AddComponent<Animator>(entity);
		animation->SetActive(true);
		animation->Queue(AnimationBase(false, 0.01f, 5.f), &render->tint.a, 1.f);
		animation->Queue(AnimationBase(false, 0.01f, 5.f), &text->color.a, 1.f);
	}
}

void ScoreScene::Destroy() {
	Scene::Destroy();
	delete circle;
}

void ScoreScene::CreateRow(float const& i, std::string const & label, std::string const & content, Font* font) {
	const float delay = 2.f + 0.5f * i;
	const float y = -1.5f * i;

	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetLocalTranslation(vec3f(0.f, y, 0.f));

		Text* const text = entities->AddComponent<Text>(entity);
		text->SetActive(true);
		text->SetFont(font);
		text->text = label + " : ";
		text->paragraphAlignment = PARAGRAPH_RIGHT;
		text->color = vec4f(1.f, 1.f, 1.f, 0.f);

		Animator* const animation = entities->AddComponent<Animator>(entity);
		animation->SetActive(true);
		animation->Queue(AnimationBase(false, 0.01f, delay), &text->color.a, 1.f);
	}

	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetLocalTranslation(vec3f(0.f, y, 0.f));

		Text* const text = entities->AddComponent<Text>(entity);
		text->SetActive(true);
		text->SetFont(font);
		text->text = "   " + content;
		text->paragraphAlignment = PARAGRAPH_LEFT;
		text->color = vec4f(1.f, 1.f, 1.f, 0.f);
		text->offset.y = -1.f;

		Animator* const animation = entities->AddComponent<Animator>(entity);
		animation->SetActive(true);
		animation->Queue(AnimationBase(false, 0.1f, delay), &text->offset.y, 0.f);
		animation->Queue(AnimationBase(false, 0.01f, delay), &text->color.a, 1.f);
	}
}
