#ifndef LINE_RENDER_H
#define LINE_RENDER_H

#include "Component.h"

#include "Material.h"

#include <Math/Vectors.h>
#include <Events/Event.h>

struct LineRender : Component {

	using base_type = LineRender;

	// size of UV in tilemap
	vec3f offset;
	vec3f length;
	vec4f tint;

	LineRender();

	void Initialize() override;
	Component* Clone() const override;
	void SetActive(bool const& state) override;
	void SetDynamic(bool const& state) override;

	Material::Base* const GetMaterial() const;
	void SetMaterial(Material::Base* _material);

private:

	Material::Base* material;

};

#endif
