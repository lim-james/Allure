#include "TitleTransition.h"

#include "SampleScene.h"

#include <Events/EventsManager.h>

void TitleTransition::FadeOut() {
	button->SetActive(false);
	emitter->SetActive(false);

	AnimationBase base = AnimationBase(false, 0.5f);

	animation->Queue(
		base,
		&titleText->color.a,
		0.f
	);

	animation->Queue(
		base,
		&titleTransform->translation.y,
		0.25f
	);

	animation->Queue(
		base,
		&buttonRender->tint.a,
		0.f
	);

	animation->Queue(
		base,
		&buttonTransform->translation.y,
		-7.25f
	);

	animation->Queue(
		base,
		&grid->tint.a,
		0.f
	);

	animation->Queue(
		base,
		&underline->tint.a,
		0.f
	);
}

void TitleTransition::Transition(unsigned target) {
	FadeOut();
	transitionDelay = 5.0f;
}

void TitleTransition::Start() {
	animation = entities->GetComponent<Animation>(entity);

	titleText = entities->GetComponent<Text>(titleTransform->entity);

	button = entities->GetComponent<Button>(buttonTransform->entity);
	buttonRender = entities->GetComponent<SpriteRender>(buttonTransform->entity);

	transitionDelay = 0.0f;
}

void TitleTransition::Update() {
	if (transitionDelay < 0.0f) {
		EventsManager::Get()->Trigger("PRESENT_SCENE", new Events::PresentScene(new SampleScene));
		transitionDelay = 0.0f;
	} else if (transitionDelay > 0.0f) {
		transitionDelay -= time->dt;
	}
}

