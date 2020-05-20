#include "ExplosiveScript.h"

void ExplosiveScript::OnDestroy() {
	Transform* const eTransform = explosionPrefab->Create();
	eTransform->translation = transform->GetWorldTranslation();
}
