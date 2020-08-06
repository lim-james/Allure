#ifndef LIGHT_2D_H
#define LIGHT_2D_H

#include "Component.h"

#include "Light.h"

struct Light2D : Light {

	using base_type = Light;

	Component* Clone() const override;

	void SetActive(bool const& state) override;

	void SetCastShadows(bool const& state) override;

private:

	mat4f GetProjectionMatrix() const override;

};


#endif
