#include "TitleTransition.h"

#include "Scene.h"

#include <Events/EventsManager.h>

void TitleTransition::FadeOut() {
	button->SetActive(false);
	emitter->SetActive(false);

	AnimationBase base = AnimationBase(false, 0.5f);

	titleAnimation->Animate(
		base,
		&titleText->color.a,
		0.f
	);

	titleAnimation->Animate(
		base,
		&titleTransform->translation.y,
		0.25f
	);

	buttonAnimation->Animate(
		base,
		&buttonRender->tint.a,
		0.f
	);

	buttonAnimation->Animate(
		base,
		&buttonTransform->translation.y,
		-7.25f
	);
}

void TitleTransition::Transition(unsigned target) {
	FadeOut();
	transitionDelay = 5.0f;
}

void TitleTransition::Start() {
	titleAnimation = entities->GetComponent<Animation>(titleTransform->entity);
	titleText = entities->GetComponent<Text>(titleTransform->entity);

	button = entities->GetComponent<Button>(buttonTransform->entity);
	buttonAnimation = entities->GetComponent<Animation>(buttonTransform->entity);
	buttonRender = entities->GetComponent<Render>(buttonTransform->entity);

	transitionDelay = 0.0f;
}

void TitleTransition::Update() {
	if (transitionDelay < 0.0f) {
		Events::EventsManager::GetInstance()->Trigger("PRESENT_SCENE", new Events::PresentScene(new Scene("Files/Scenes/Demo.scene")));
		transitionDelay = 0.0f;
	} else if (transitionDelay > 0.0f) {
		transitionDelay -= time->dt;
	}
}

