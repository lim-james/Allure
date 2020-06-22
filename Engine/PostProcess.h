#ifndef POST_PROCESS_H
#define POST_PROCESS_H

#include "Component.h"

#include <Handler/Handler.h>

struct PostProcess : Component {

	using base_type = PostProcess;

	static unsigned VAO;

	PostProcess();

	void SetActive(bool const& state) override;

	virtual void PreRender() = 0;
	virtual void PostRender() = 0;
	virtual void Render() = 0;

};

#endif
