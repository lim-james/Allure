#ifndef EDITOR_MANAGER_H
#define EDITOR_MANAGER_H

#include "Script.h"
#include "Text.h"
#include "SpriteRender.h"
#include "AudioSource.h"
#include "Animator.h"
#include "TransformAnimator.h"
#include "Layout.h"
#include "AVController.h"

struct EditorManager : Script {

	using base_type = EditorManager;

	Text* promptLabel;

	SpriteRender* background;
	
	// animators

	Animator* animator;
	TransformAnimator* tAnimator;
	
	// av stuff

	Layout* avLayout;
	AVController* controller;

private: 

	float alertBt;

	void Awake() override;
	void Start() override;
	void Update() override;

	void DropHandler(Events::Event* event);

};

#endif
