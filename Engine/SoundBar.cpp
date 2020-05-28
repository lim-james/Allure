#include "SoundBar.h"

#include "SpriteRender.h"

Transform * SoundBar::Create() {
	const unsigned entity = entities->Create();

	SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
	render->SetActive(true);
	render->tint = vec4f(1.f, 0.f, 0.5f, 1.f);

	return entities->GetComponent<Transform>(entity);
}
