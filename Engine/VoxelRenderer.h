#ifndef VOXEL_RENDERER_H
#define VOXEL_RENDERER_H

#include "Renderer.h"
#include "VoxelDefaultMaterial.h"
#include "VoxelRender.h"
#include "Model.h"

#include <Events/Event.h>

class VoxelRenderer : public Renderer {

	struct Instance {
		vec4f tint;
		vec3f position;
	};

	struct Batch {
		struct StaticData {
			unsigned VAO, count;
			StaticData();
		};

		std::map<Camera*, StaticData> staticData;
		std::vector<VoxelRender*> staticList;
		std::vector<VoxelRender*> dynamicList;
	};

	typedef std::map<Shader*, std::map<Material::Base*, Batch>> Batches;

	static const unsigned INSTANCE_LAYOUT_LOCATION = 1;
	static Mesh* cube;
	static unsigned dynamicVAO;
	static unsigned dynamicBuffer;

	Material::VoxelDefault* defaultMaterial;

	bool updateStatic;
	Batches opaqueBatches;
	Batches transparentBatches;

public:

	~VoxelRenderer() override;

	void Initialize(EntityManager* const manager) override;

	void RenderDepth(RendererData const& data) override;
	void RenderOpaque(RendererData const& data) override;
	void RenderTransparent(RendererData const& data) override;
	void PostRender() override;

private:

	void InitializeInstanceBuffer(unsigned const& VAO, unsigned& instanceBuffer);

	void RenderBatches(RendererData const& data, Batches& batches);
	void RenderStatic(RendererData const& data, Batch& batch);
	void RenderDynamic(RendererData const& data, Batch const& batch);

	void ActiveHandler(Events::Event* event);
	void DynamicHandler(Events::Event* event);
	void MaterialHandler(Events::Event* event);
	void ShaderHandler(Events::Event* event);

};

#endif
