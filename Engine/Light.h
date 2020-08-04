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

	virtual void Initialize();
	virtual Component* Clone() const;

	virtual void SetActive(bool const& state);

	virtual mat4f GetProjectionMatrix() const;

	virtual bool const& CastShadows() const;
	virtual void SetCastShadows(bool const& state);

protected:

	bool castShadows;

};

#endif
