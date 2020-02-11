#ifndef EDITOR_SCENE_H
#define EDITOR_SCENE_H

#include "Scene.h"

#include "Render.h"
#include "Text.h"
#include "ParticleEmitter.h"

class EditorScene : public Scene {

	std::string destination;
	float transitionDelay;

	ParticleEmitter* emitter;
	std::string filepath;
	time_t lastMod;

	Text* titleText;
	Render* buttonRender;

public:

	void Awake();
	void Start();

	void FixedUpdate(const float& dt) override;

	void SetFile(const std::string& _filepath);

private:

	void DropHandler(Events::Event * event);

	void OnMouseOver(unsigned target);
	void OnMouseOut(unsigned target);
	void OnMouseDown(unsigned target);
	void OnMouseUp(unsigned target);
	void OnClick(unsigned target);

	void UpdateEmitter();
	void FadeOut();

};

#endif
