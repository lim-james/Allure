#include "BackgroundMaterial.h"

Material::Background::Background() 
	: et(0.f)
	, speed(1.f)
	// spread
	, spreadWeight(1.f)
	, spreadAlpha(1.f)
	// indicator
	, indicatorWeight(0.05f)
	, indicatorAlpha(1.f)
	, indicatorTint(1.f)
	, viewSize(1.f)
	, interval(1.f) 
	, bt(0.f)
	// threshold
	, thresholdWeight(0.05f)
	, thresholdRadius(1.f)
	, thresholdAlpha(1.f)
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
	//spread
	shader->SetFloat("spreadWeight", spreadWeight);
	shader->SetFloat("spreadAlpha", spreadAlpha);
	const int count = static_cast<int>(spreads.size());
	shader->SetInt("spreadCount", count);
	for (int i = 0; i < count; ++i) {
		const std::string prefix = "spreads[" + std::to_string(i) + "].";
		shader->SetFloat(prefix + "et", spreads[i].et);
		shader->SetVector3(prefix + "tint", spreads[i].tint);
	}
	// indicator
	shader->SetFloat("indicatorWeight", indicatorWeight);
	shader->SetFloat("indicatorRadius", indicatorRadius);
	shader->SetFloat("indicatorAlpha", indicatorAlpha);
	shader->SetVector3("indicatorTint", indicatorTint);
	shader->SetFloat("viewSize", viewSize);
	shader->SetFloat("interval", interval);
	shader->SetFloat("bt", bt);
	// threshold
	shader->SetFloat("thresholdWeight", thresholdWeight);
	shader->SetFloat("thresholdRadius", thresholdRadius);
	shader->SetFloat("thresholdAlpha", thresholdAlpha);
	shader->SetVector3("thresholdTint", thresholdTint);
	shader->SetFloat("threshold", threshold);

	shader->SetVector3("playerPosition", playerPosition);
}
