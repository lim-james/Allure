#ifndef POST_PROCESS_STACK_H
#define POST_PROCESS_STACK_H

#include "PostProcess.h"

#include <vector>

class PostProcessStack {

	std::vector<PostProcess*> layers;

public:

	Handler<void, void> rawRender;

	~PostProcessStack();

	void Render();
	void Push(PostProcess* const layer);

};

#endif
