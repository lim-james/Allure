#ifndef AV_BUTTON_H
#define AV_BUTTON_H

#include "Prefab.h"
#include "Material.h"

struct AVButton : Prefab {
	Material::Base* material;
	Transform* Create() override;
};

#endif
