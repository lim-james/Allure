#include "PostProcessStack.h"

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
		start->Process();
	}
}

void PostProcessStack::Push(PostProcess * const layer) {
	if (!layers.empty())
		layers.back()->parent = layer;

	layers.push_back(layer);
}
