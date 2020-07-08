#include "ConeMaterial.h"

Material::Cone::Cone() 
	: borderWeight(0.1f) 
	, range(15.f * Math::toRad) {
	shader = new Shader("Files/Shaders/standard2D.vert", "Files/Shaders/cone.frag");
}

void Material::Cone::SetRange(float const & deg) {
	range = deg * Math::toRad;
}

void Material::Cone::SetAttributes() {
	shader->SetFloat("borderWeight", borderWeight);
	shader->SetFloat("range", range);
}
