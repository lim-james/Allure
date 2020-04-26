#ifndef POST_PROCESS_H
#define POST_PROCESS_H

#include "Component.h"

#include <Handler/Handler.h>

struct PostProcess : Component {

	PostProcess();

	void SetActive(bool const& state) override;

	virtual void PreRender() = 0;
	virtual void PostRender() = 0;
	virtual void Render() = 0;

protected:

	static unsigned VAO;

};

#endif
