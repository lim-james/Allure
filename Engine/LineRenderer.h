#ifndef LINE_RENDERER_H
#define LINE_RENDERER_H

#include "Renderer.h"
#include "LineRender.h"
#include "LineDefaultMaterial.h"

#include <Events/Event.h>

class LineRenderer: public Renderer {

	struct Instance {
		vec3f offset;
		vec3f length;
		vec4f tint;
		mat4f model;
	};

	typedef std::map<Shader*, std::map<Material::Base*, std::vector<LineRender*>>> Batches;

	static const unsigned INSTANCE_LAYOUT_LOCATION = 1;
	static unsigned lineVAO;
	static unsigned instanceBuffer;

	Material::LineDefault* defaultMaterial;
	Batches batches;

public:

	~LineRenderer() override;

	void Initialize(EntityManager* const manager) override;
	void Render(RendererData const& data) override;

private:

	void ActiveHandler(Events::Event* event);
	void MaterialHandler(Events::Event* event);
	void ShaderHandler(Events::Event* event);
		
	void GenerateLine();

};

#endif
