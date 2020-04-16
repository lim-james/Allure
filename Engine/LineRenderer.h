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

	struct Batch {
		struct StaticData {
			unsigned VAO, count;
			StaticData();
		};

		std::map<Camera*, StaticData> staticData;
		std::vector<LineRender*> staticList;
		std::vector<LineRender*> dynamicList;
	};

	typedef std::map<Shader*, std::map<Material::Base*, Batch>> Batches;

	static const unsigned INSTANCE_LAYOUT_LOCATION = 1;
	static unsigned dynamicVAO;
	static unsigned dynamicBuffer;

	Material::LineDefault* defaultMaterial;

	bool updateStatic;
	Batches batches;

public:

	~LineRenderer() override;

	void Initialize(EntityManager* const manager) override;
	void Render(RendererData const& data) override;

private:

	void InitializeInstanceBuffer(unsigned& instanceBuffer);

	void RenderStatic(RendererData const& data, Batch& batch);
	void RenderDynamic(RendererData const& data, Batch const& batch);

	void ActiveHandler(Events::Event* event);
	void DynamicHandler(Events::Event* event);
	void MaterialHandler(Events::Event* event);
	void ShaderHandler(Events::Event* event);
		
	void GenerateLine(unsigned& VAO);

};

#endif
