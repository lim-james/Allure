#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include "Renderer.h"
#include "MeshDefaultMaterial.h"
#include "MeshRender.h"

#include <Events/Event.h>

class MeshRenderer : public Renderer {

	struct Batch {
		struct StaticData {
			unsigned VAO, count;
			StaticData();
		};

		std::map<void*, StaticData> staticData;
		std::vector<MeshRender*> staticList;
		std::vector<MeshRender*> dynamicList;
	};

	typedef std::map<Mesh*, Batch> MeshBatches;
	typedef std::map<Shader*, std::map<Material::Base*, MeshBatches>> Batches;

	static const unsigned INSTANCE_LAYOUT_LOCATION = 3;

	Shader* depthShader;
	Material::MeshDefault* defaultMaterial;

	std::map<Mesh*, unsigned> dynamicBuffers;
	bool updateStatic;
	Batches opaqueBatches;
	Batches transparentBatches;

public:

	~MeshRenderer() override;

	void Initialize(EntityManager* const manager) override;
	void RenderDepth(RendererData const& data) override;
	void RenderOpaque(RendererData const& data) override;
	void RenderTransparent(RendererData const& data) override;
	void PostRender() override;

private:

	void InitializeInstanceBuffer(unsigned const& VAO, unsigned& instanceBuffer);

	void RenderDepthBatches(RendererData const& data, Batches& batches);

	void RenderBatches(RendererData const& data, Batches& batches);
	void RenderStatic(RendererData const& data, Mesh* const mesh, Batch& batch);
	void RenderDynamic(RendererData const& data, Mesh* const mesh, Batch const& batch);

	void ActiveHandler(Events::Event* event);
	void DynamicHandler(Events::Event* event);

	void ModelChangeHandler(Events::Event* event);
	void MaterialHandler(Events::Event* event);
	void ShaderHandler(Events::Event* event);

	void CastShadowHandler(Events::Event* event);

};


#endif
