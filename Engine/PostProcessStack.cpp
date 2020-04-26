#include "PostProcessStack.h"

#include <Helpers/VectorHelpers.h>
#include <Events/EventsManager.h>
#include <GL/glew.h>

PostProcessStack::PostProcessStack() {
	Events::EventsManager::GetInstance()->Subscribe("POST_PROCESS_ACTIVE", &PostProcessStack::ActiveHanlder, this);
}

PostProcessStack::~PostProcessStack() {
	for (PostProcess* const layer : layers)
		delete layer;
}

void PostProcessStack::Render() {

	if (layers.empty()) {
		rawRender();
	} else {
		PostProcess* const start = layers.front();
		start->PreRender();
		rawRender();
		start->PostRender();
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glDisable(GL_DEPTH_TEST);
		Process(1);
	}
	glEnable(GL_DEPTH_TEST);
}

void PostProcessStack::ActiveHanlder(Events::Event * event) {
	PostProcess* const c = static_cast<Events::AnyType<PostProcess*>*>(event)->data;
	if (c->IsActive()) {
		Helpers::Insert(layers, c);
	} else {
		Helpers::Remove(layers, c);
	}
}

void PostProcessStack::Process(unsigned const& index) {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	if (layers.size() > index) {
		PostProcess* const parent = layers[index];
		parent->PreRender();
		layers[index - 1]->Render();
		parent->PostRender();
		Process(index + 1);
	} else {
		layers[index - 1]->Render();
	}
}
