#include "LaserPath.h"

#include "SpriteRender.h"
#include "Physics.h"
#include "LineCollider.h"
#include "SelfDestruct.h"

#include "LoadTexture.h"
#include "Layers.h"

Transform* LaserPath::Create() {
	const unsigned entity = entities->Create();
	entities->SetLayer(entity, BULLET);

	Transform* const transform = entities->GetComponent<Transform>(entity);
	transform->scale.xy = vec2f(40.f, 0.5f);

	SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
	render->SetActive(true);
	render->tint = vec4f(1.f, 0.f, 0.f, 1.f);

	LineCollider* const collider = entities->AddComponent<LineCollider>(entity);
	collider->SetActive(true);
	collider->ignoreMask = PLAYER & WEAPON;

	return transform;
}
