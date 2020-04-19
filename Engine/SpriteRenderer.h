#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "Renderer.h"
#include "SpriteDefaultMaterial.h"
#include "SpriteRender.h"

#include <Events/Event.h>

class SpriteRenderer : public Renderer {

	struct Instance {
		vec4f uvRect;
		vec4f tint;
		mat4f model;
	};

	struct Batch {
		struct StaticData {
			unsigned VAO, count;
			StaticData();
		};

		std::map<Camera*, StaticData> staticData;
		std::vector<SpriteRender*> staticList;
		std::vector<SpriteRender*> dynamicList;
	};

	typedef std::map<unsigned, Batch> SpriteBatches;
	typedef std::map<Shader*, std::map<Material::Base*, SpriteBatches>> Batches;

	static const unsigned INSTANCE_LAYOUT_LOCATION = 2;
	static unsigned dynamicVAO;
	static unsigned dynamicBuffer;

	Material::SpriteDefault* defaultMaterial;

	bool updateStatic;
	Batches opaqueBatches;
	Batches transparentBatches;

public:

	~SpriteRenderer() override;

	void Initialize(EntityManager* const manager) override;
	void Render(RendererData const& data) override;

private:

	void InitializeInstanceBuffer(unsigned const& VAO, unsigned& instanceBuffer);

	void RenderBatches(RendererData const& data, Batches& batches);
	void RenderStatic(RendererData const& data, Batch& batch);
	void RenderDynamic(RendererData const& data, Batch const& batch);

	void ActiveHandler(Events::Event* event);
	void DynamicHandler(Events::Event* event);
	void SpriteChangeHandler(Events::Event* event);
	void MaterialHandler(Events::Event* event);
	void ShaderHandler(Events::Event* event);

	void GenerateQuad(unsigned& VAO);

};

#endif
