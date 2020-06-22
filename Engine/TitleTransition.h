#ifndef TITLE_TRANSITION_H
#define TITLE_TRANSITION_H

#include "Script.h"

#include "ParticleEmitter.h"
#include "Animation.h"
#include "Text.h"
#include "SpriteRender.h"
#include "LineRender.h"
#include "Button.h"

struct TitleTransition : Script {

	using base_type = TitleTransition;

	ParticleEmitter* emitter;
	Transform* titleTransform;
	Transform* buttonTransform;
	SpriteRender* grid;
	LineRender* underline;

private:

	// references

	Animation* animation;

	Text* titleText;

	Button* button;
	SpriteRender* buttonRender;

	// attributes

	float transitionDelay;

public:

	void FadeOut();
	void Transition(unsigned target);

private:

	void Start() override;
	void Update() override;

};

#endif
