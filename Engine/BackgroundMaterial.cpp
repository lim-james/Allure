#include "BackgroundMaterial.h"

Material::Background::Background() 
	: et(0.f)
	, speed(1.f)
	, spread(1.f)
	, spreadTint(1.f)
	, indicatorTint(1.f)
	, viewSize(1.f)
	, interval(1.f) 
	, bt(0.f)
	, thresholdTint(1.f)
	, threshold(0.f)
	, playerPosition(0.f) {
	shader = new Shader("Files/Shaders/background.vert", "Files/Shaders/background.frag");
	shader->Use();
	shader->SetInt("tex", 0);
}

void Material::Background::SetAttributes() {
	shader->SetFloat("et", et);
	shader->SetFloat("speed", speed);
	shader->SetFloat("spread", spread);
	shader->SetVector3("spreadTint", spreadTint);
	shader->SetVector3("indicatorTint", indicatorTint);
	shader->SetFloat("viewSize", viewSize);
	shader->SetFloat("interval", interval);
	shader->SetFloat("bt", bt);
	shader->SetVector3("thresholdTint", thresholdTint);
	shader->SetFloat("threshold", threshold);
	shader->SetVector3("playerPosition", playerPosition);
}
