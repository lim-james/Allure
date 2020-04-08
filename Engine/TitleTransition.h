#ifndef TITLE_TRANSITION_H
#define TITLE_TRANSITION_H

#include "Script.h"

#include "ParticleEmitter.h"
#include "Animation.h"
#include "Text.h"
#include "SpriteRender.h"
#include "Button.h"

struct TitleTransition : Script {

	ParticleEmitter* emitter;
	Transform* titleTransform;
	Transform* buttonTransform;
	SpriteRender* grid;

private:

	// references

	Animation* titleAnimation;
	Text* titleText;

	Button* button;
	Animation* buttonAnimation;
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
