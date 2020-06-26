#include "PostProcessStack.h"

#include <Helpers/VectorHelpers.h>
#include <Events/EventsManager.h>
#include <GL/glew.h>

PostProcessStack::PostProcessStack() {
	EventsManager::Get()->Subscribe("POST_PROCESS_ACTIVE", &PostProcessStack::ActiveHanlder, this);
	EventsManager::Get()->Subscribe("WINDOW_RESIZE", &PostProcessStack::ResizeHandler, this);
}

void PostProcessStack::Render() {
	if (layers.empty()) {
		glViewport(0, 0, windowSize.w, windowSize.h);
		glScissor(0, 0, windowSize.w, windowSize.h);
		rawRender.Invoke();
	} else {
		PostProcess* const start = layers.front();
		start->PreRender();
		rawRender.Invoke();
		start->PostRender();
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glDisable(GL_DEPTH_TEST);
		Process(1);
		glEnable(GL_DEPTH_TEST);
	}
}

void PostProcessStack::ActiveHanlder(Events::Event * event) {
	PostProcess* const c = static_cast<Events::AnyType<PostProcess*>*>(event)->data;
	if (c->IsActive()) {
		Helpers::Insert(layers, c);
	} else {
		Helpers::Remove(layers, c);
	}
}

void PostProcessStack::ResizeHandler(Events::Event * event) {
	windowSize = static_cast<Events::AnyType<vec2i>*>(event)->data;
}

void PostProcessStack::Process(unsigned const& index) {
	if (layers.size() > index) {
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		PostProcess* const parent = layers[index];
		parent->PreRender();
		layers[index - 1]->Render();
		parent->PostRender();
		Process(index + 1);
	} else {
		glViewport(0, 0, windowSize.w, windowSize.h);
		glScissor(0, 0, windowSize.w, windowSize.h);
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		layers[index - 1]->Render();
	}
}
