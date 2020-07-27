#ifndef TILEMAP_RENDERER_H
#define TILEMAP_RENDERER_H

#include "Renderer.h"
#include "TilemapDefaultMaterial.h"
#include "TilemapRender.h"

#include <Events/Event.h>

class TilemapRenderer : public Renderer {

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

		std::map<void*, StaticData> staticData;
		std::vector<TilemapRender*> staticList;
		std::vector<TilemapRender*> dynamicList;
	};

	typedef std::map<unsigned, Batch> TilemapBatches;
	typedef std::map<Shader*, std::map<Material::Base*, TilemapBatches>> Batches;

	static const unsigned INSTANCE_LAYOUT_LOCATION = 2;
	static unsigned dynamicVAO;
	static unsigned dynamicBuffer;

	Shader* depthShader;
	Material::TilemapDefault* defaultMaterial;

	bool updateStatic;
	Batches opaqueBatches;
	Batches transparentBatches;

public:

	~TilemapRenderer() override;

	void Initialize(EntityManager* const manager) override;

	void RenderDepth(RendererData const& data) override;
	void RenderOpaque(RendererData const& data) override;
	void RenderTransparent(RendererData const& data) override;
	void PostRender() override;

	static void GenerateQuad(unsigned& VAO);

private:

	void InitializeInstanceBuffer(unsigned const& VAO, unsigned& instanceBuffer);

	void RenderDepthBatches(RendererData const& data, Batches& batches);

	void RenderBatches(RendererData const& data, Batches& batches);
	void RenderStatic(RendererData const& data, unsigned const& texture, Batch& batch);
	void RenderDynamic(RendererData const& data, unsigned const& texture, Batch const& batch);

	void ActiveHandler(Events::Event* event);
	void DynamicHandler(Events::Event* event);
	//void TilemapChangeHandler(Events::Event* event);
	void MaterialHandler(Events::Event* event);
	void ShaderHandler(Events::Event* event);

};

#endif
