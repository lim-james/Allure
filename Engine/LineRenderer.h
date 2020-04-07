#ifndef LINE_RENDERER_H
#define LINE_RENDERER_H

#include "Renderer.h"
#include "Shader.h"
#include "Line.h"

#include <Events/Event.h>

class LineRenderer: public Renderer {

	static unsigned instanceBuffer;
	static unsigned lineVAO;

	Shader* shader;
	std::vector<Line> lines;

public:

	~LineRenderer() override;

	void Initialize(EntityManager* const manager) override;
	void Render(RendererData const& data) override;

private:

	void DrawLineHandler(Events::Event* event);
		
	void GenerateLine();

};

#endif
