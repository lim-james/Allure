#ifndef POST_PROCESS_STACK_H
#define POST_PROCESS_STACK_H

#include "PostProcess.h"

#include <Math/Vectors.h>
#include <Events/Event.h>
#include <vector>

class PostProcessStack {

	std::vector<PostProcess*> layers;
	vec2i windowSize;

public:

	Handler<void, void> rawRender;

	PostProcessStack();

	void Render();

private:

	void ActiveHanlder(Events::Event* event);
	void ResizeHandler(Events::Event* event);

	void Process(unsigned const& index);

};

#endif
