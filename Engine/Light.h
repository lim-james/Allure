#ifndef LIGHT_H
#define LIGHT_H

#include "Component.h"

#include <Math/Vectors.h>
#include <Math/Mat4.hpp>
#include <Bit/BitField.h>

#define LIGHT_SPOT			0
#define LIGHT_DIRECTIONAL	1
#define LIGHT_POINT			2

struct Light : Component {

	using base_type = Light;

	short type;

	float range;
	float innerCutOff, outerCutOff;

	vec3f color;
	float intensity;

	// shadow attributes
	float strength;
	BitField cullingMask;

	unsigned shadowMap;

	Light();

	void Initialize() override;
	Component* Clone() const override;

	void SetActive(bool const& state) override;

	mat4f GetProjectionMatrix() const;

	bool const& CastShadows() const;
	void SetCastShadows(bool const& state);

private:

	bool castShadows;

};

#endif
