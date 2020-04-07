#ifndef STANDARD_RENDERER_H
#define STANDARD_RENDERER_H

#include "Renderer.h"
#include "Shader.h"
#include "Render.h"

#include <Events/Event.h>

struct Instance {
	Render* component;
	vec4f uvRect;
	vec4f tint;
	mat4f model;
};

bool operator==(const Instance& lhs, const Instance& rhs);

typedef std::map<unsigned, std::vector<Instance>> Batches;

class StandardRenderer: public Renderer {

	static const unsigned INSTANCE_LAYOUT_LOCATION = 2;
	static unsigned instanceBuffer;
	static unsigned quadVAO;

	Shader* shader;
	Batches batches;

public:

	~StandardRenderer() override;

	void Initialize(EntityManager* const manager) override;
	void Render(const RendererData& data) override;

private:

	void ActiveHandler(Events::Event* event);
	void TextureChangeHandler(Events::Event* event);
		
	void GenerateQuad();

};

#endif
