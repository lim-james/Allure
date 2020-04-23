#include "PortalCamera.h"

#include <Math/Mat4Transform.hpp>

void PortalCamera::Update() {
	mat4f transformMatrix;
	Math::SetToIdentity(transformMatrix);
	Math::Translate(transformMatrix, -portal1->GetWorldTranslation());
	Math::Rotate(transformMatrix, -portal1->GetWorldRotation());
	Math::Rotate(transformMatrix, portal2->GetWorldRotation());
	Math::Translate(transformMatrix, portal2->GetWorldTranslation());

	mat4f rotationMatrix;
	Math::SetToIdentity(rotationMatrix);
	Math::Rotate(rotationMatrix, -portal1->GetWorldRotation());
	Math::Rotate(rotationMatrix, portal2->GetWorldRotation());

	transform->translation = transformMatrix * mainCamera->GetWorldTranslation();
	transform->rotation = rotationMatrix * mainCamera->GetWorldRotation();
	transform->UpdateAxes();
}
