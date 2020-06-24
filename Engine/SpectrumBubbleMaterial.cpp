#include "SpectrumBubbleMaterial.h"

Material::SpectrumBubble::SpectrumBubble() 
	: minRadius(0.5f)
	, maxValue(1.f)
	, unit(1.f)
	, outlineWeight(0.1f)
	, outlineColor(1.f, 0.f, 0.f)
	, multiplier(1.f) {
	shader = new Shader("Files/Shaders/standard2D.vert", "Files/Shaders/spectrumBubble.frag");
}

void Material::SpectrumBubble::SetAttributes() {
	shader->SetFloat("minRadius", minRadius);
	shader->SetFloat("maxValue", maxValue);
	shader->SetFloat("unit", unit);
	shader->SetFloat("multiplier", multiplier);
	shader->SetFloat("outlineWeight", outlineWeight);
	shader->SetVector3("outlineColor", outlineColor);

	for (unsigned i = 0; i < values.size(); ++i)
		shader->SetVector2("values[" + std::to_string(i) + ']', vec2f(values[i]));
}
