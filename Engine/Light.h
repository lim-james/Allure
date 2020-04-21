#ifndef LIGHT_H
#define LIGHT_H

#include "Component.h"

#include <Math/Vectors.h>
#include <Bit/BitField.h>

#define LIGHT_SPOT			0
#define LIGHT_DIRECTIONAL	1
#define LIGHT_POINT			2

struct Light : Component {

	short type;

	float range;
	float innerCutOff, outerCutOff;

	vec4f color;
	float intensity;
	float indirectMultiplier;

	bool castShadow;
	float strength;

	BitField cullingMask;

	Light();

	void Initialize() override;
	Component* Clone() const override;

	void SetActive(bool const& state) override;

};

#endif
