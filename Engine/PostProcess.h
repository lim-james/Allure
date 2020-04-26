#ifndef POST_PROCESS_H
#define POST_PROCESS_H

#include <Handler/Handler.h>

struct PostProcess {

	PostProcess* parent;

	PostProcess();

	virtual void PreRender() = 0;
	virtual void PostRender() = 0;
	void Process();

protected:

	static unsigned VAO;

	virtual void Render() = 0;

};

#endif
