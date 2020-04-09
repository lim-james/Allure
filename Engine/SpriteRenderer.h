#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "Renderer.h"
#include "Shader.h"
#include "SpriteRender.h"

#include <Events/Event.h>

struct Instance {
	vec4f uvRect;
	vec4f tint;
	mat4f model;
};

typedef std::map<unsigned, std::vector<SpriteRender*>> Batches;

class SpriteRenderer: public Renderer {

	static const unsigned INSTANCE_LAYOUT_LOCATION = 2;
	static unsigned instanceBuffer;
	static unsigned quadVAO;

	Shader* shader;
	Batches batches;

public:

	~SpriteRenderer() override;

	void Initialize(EntityManager* const manager) override;
	void Render(RendererData const& data) override;

private:

	void ActiveHandler(Events::Event* event);
	void SpriteChangeHandler(Events::Event* event);
		
	void GenerateQuad();

};

#endif
