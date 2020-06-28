#include "ScoreScene.h"

#include "SpriteRender.h"
#include "Vignette.h"
#include "ProjectDefines.h"
#include "LayoutSystem.h"

void ScoreScene::Awake() {
	Scene::Awake();

	systems->Subscribe<LayoutSystem>(1);
}

void ScoreScene::Create() {
	Scene::Create();

	Camera* const camera = entities->GetComponent<Camera>(mainCamera);

	// post processing volume
	{
		const unsigned volume = entities->Create();
		Vignette* const vignette = entities->AddComponent<Vignette>(volume);
		vignette->SetActive(true);
		vignette->tint = COLOR_PURPLE; // vec3f(0.5f, 0.f, 0.5f);
		//entities->AddComponent<CurveDisplay>(volume)->SetActive(true);
	}

	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetScale(vec3f(16.f, 9.f, 1.f));

		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);
		render->SetSprite(sceneTexture);

		Layout* const layout = entities->AddComponent<Layout>(entity);
		layout->SetActive(true);
		layout->AddConstraint(Constraint(WIDTH, nullptr, WIDTH, 1.f, 0.f, camera));
		layout->AddConstraint(Constraint(HEIGHT, nullptr, HEIGHT, 1.f, 0.f, camera));
	}
}
