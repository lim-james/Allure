#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include "Renderer.h"
#include "Shader.h"
#include "Text.h"

#include <Events/Event.h>

class TextRenderer: public Renderer {

	Shader* shader;
	std::map<Font*, std::vector<Text*>> textBatches;

public:

	~TextRenderer() override;

	void Initialize(EntityManager* const manager) override;

	void RenderDepth(RendererData const& data) override;
	void RenderOpaque(RendererData const& data) override;
	void RenderTransparent(RendererData const& data) override;

private:

	void ActiveHandler(Events::Event* event);
	void FontHandler(Events::Event* event);

};

#endif
