#include "IndicatorLabel.h"

#include "Text.h"
#include "Physics.h"
#include "SphereCollider.h"
#include "Animation.h"
#include "SelfDestruct.h"
#include "Layers.h"
#include "LoadFNT.h"

Transform* IndicatorLabel::Create() {
	Font* const vcrMono = Load::FNT("Files/Fonts/vcr_ocd_mono.fnt", "Files/Fonts/vcr_ocd_mono.png");

	const unsigned entity = entities->Create();
	entities->SetLayer(entity, UI);

	Transform* const transform = entities->GetComponent<Transform>(entity);

	Text* const text = entities->AddComponent<Text>(entity);
	text->SetActive(true);
	text->SetFont(vcrMono);
	text->scale = 2.f;
	text->text = "INSERT TEXT HERE";
	text->color.a = 0.f;

	Animation* const animation = entities->AddComponent<Animation>(entity);
	animation->SetActive(true);
	animation->Animate(AnimationBase(false, 0.25f, 0.f, Handler<void, void>([this, entity]() {
		//entities->Destroy(entity);
	})), &text->color.a, 1.f);

	return transform;
}
