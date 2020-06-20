#include "ExplosionArea.h"

#include "SpriteRender.h"
#include "SphereCollider.h"
#include "ExpandScript.h"
#include "SelfDestruct.h"

#include "Layers.h"

Transform * ExplosionArea::Create() {
	const unsigned entity = entities->Create();
	entities->SetLayer(entity, EFFECT_AREA);

	Transform* const transform = entities->GetComponent<Transform>(entity);

	SpriteRender* const sprite = entities->AddComponent<SpriteRender>(entity);
	sprite->SetActive(true);

	SphereCollider* const collider = entities->AddComponent<SphereCollider>(entity);
	collider->SetActive(true);
	collider->ignoreMask = BULLET & BONUS_BULLET & EFFECT_AREA;

	ExpandScript* const expand = entities->AddComponent<ExpandScript>(entity);
	expand->SetActive(true);
	expand->targetSize = vec3f(10.f, 10.f, 1.f);
	expand->duration = 0.25f;

	SelfDestruct* const destruct = entities->AddComponent<SelfDestruct>(entity);
	destruct->SetActive(true);
	destruct->lifetime = 0.25f;

	return transform;
}
