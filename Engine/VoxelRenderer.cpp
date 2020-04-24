#include "VoxelRenderer.h"

#include "Transform.h"
#include "LoadOBJ.h"

#include <Helpers/VectorHelpers.h>
#include <Events/EventsManager.h>
#include <GL/glew.h>

Mesh* VoxelRenderer::cube = nullptr;
unsigned VoxelRenderer::dynamicVAO = 0;
unsigned VoxelRenderer::dynamicBuffer = 0;

VoxelRenderer::Batch::StaticData::StaticData() : VAO(0), count(0) { }

VoxelRenderer::~VoxelRenderer() {
	delete defaultMaterial;
}

void VoxelRenderer::Initialize(EntityManager * const manager) {
	Renderer::Initialize(manager);

	if (!cube) {
		cube = Load::OBJ("Files/Models/cube.obj")->meshes[0];
	}

	if (dynamicVAO == 0) {
		dynamicVAO = cube->GenerateVAO();
	}

	InitializeInstanceBuffer(dynamicVAO, dynamicBuffer);

	defaultMaterial = new Material::VoxelDefault;

	Events::EventsManager::GetInstance()->Subscribe("VOXEL_RENDER_ACTIVE", &VoxelRenderer::ActiveHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("VOXEL_RENDER_DYNAMIC", &VoxelRenderer::DynamicHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("VOXEL_MATERIAL", &VoxelRenderer::MaterialHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("MATERIAL_SHADER", &VoxelRenderer::ShaderHandler, this);
}

void VoxelRenderer::Render(RendererData const& data) {
	RenderBatches(data, opaqueBatches);
	RenderBatches(data, transparentBatches);
	updateStatic = false;
}

void VoxelRenderer::InitializeInstanceBuffer(unsigned const& VAO, unsigned& instanceBuffer) {
	if (instanceBuffer) return;

	const unsigned stride = sizeof(Instance);

	glBindVertexArray(VAO);

	glGenBuffers(1, &instanceBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, stride, 0);
	glVertexAttribDivisor(3, 1);

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(vec4f));
	glVertexAttribDivisor(4, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VoxelRenderer::RenderBatches(RendererData const& data, Batches& batches) {
	for (auto& shaderPair : batches) {
		Shader* const shader = shaderPair.first;

		shader->Use();
		shader->SetMatrix4("projection", data.projection);
		shader->SetMatrix4("view", data.view);

		for (auto& materialPair : shaderPair.second) {
			materialPair.first->Use();
			RenderStatic(data, materialPair.second);
			RenderDynamic(data, materialPair.second);
		}
	}
}

void VoxelRenderer::RenderStatic(RendererData const & data, Batch& batch) {
	Batch::StaticData& staticData = batch.staticData[data.camera];

	if (updateStatic) {
		if (batch.staticList.empty()) return;

		std::vector<Instance> instances;
		instances.reserve(batch.staticList.size());

		for (VoxelRender* const c : batch.staticList) {
			if (entities->GetLayer(c->entity) != data.camera->cullingMask) {
				continue;
			}

			Transform* const transform = entities->GetComponent<Transform>(c->entity);

			Instance instance;
			instance.tint = c->tint;
			instance.position = transform->GetWorldTranslation();
			instances.push_back(instance);
		}

		if (instances.empty()) return;

		if (staticData.VAO == 0)
			staticData.VAO = cube->GenerateVAO();

		unsigned VBO = 0;
		InitializeInstanceBuffer(staticData.VAO, VBO);
		staticData.count = instances.size();

		glBindVertexArray(staticData.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, staticData.count * sizeof(Instance), &instances[0], GL_STATIC_DRAW);
		glDrawElementsInstanced(GL_TRIANGLES, cube->indicesSize, GL_UNSIGNED_INT, (void*)(0), staticData.count);
	} else {
		glBindVertexArray(staticData.VAO);
		glDrawElementsInstanced(GL_TRIANGLES, cube->indicesSize, GL_UNSIGNED_INT, (void*)(0), staticData.count);
	}

}

void VoxelRenderer::RenderDynamic(RendererData const& data, Batch const& batch) {
	if (batch.dynamicList.empty()) return;

	std::vector<Instance> instances;
	instances.reserve(batch.dynamicList.size());

	for (VoxelRender* const c : batch.dynamicList) {
		if (entities->GetLayer(c->entity) != data.camera->cullingMask) {
			continue;
		}

		Transform* const transform = entities->GetComponent<Transform>(c->entity);

		Instance instance;
		instance.tint = c->tint;
		instance.position = transform->GetWorldTranslation();
		instances.push_back(instance);
	}

	if (instances.empty()) return;

	const unsigned count = instances.size();
	glBindVertexArray(dynamicVAO);
	glBindBuffer(GL_ARRAY_BUFFER, dynamicBuffer);
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(Instance), &instances[0], GL_STATIC_DRAW);
	glDrawElementsInstanced(GL_TRIANGLES, cube->indicesSize, GL_UNSIGNED_INT, (void*)(0), count);
}

void VoxelRenderer::ActiveHandler(Events::Event * event) {
	VoxelRender* const c = static_cast<Events::AnyType<VoxelRender*>*>(event)->data;

	if (!c->IsDynamic()) updateStatic = true;

	Material::Base* const material = c->GetMaterial() ? c->GetMaterial() : defaultMaterial;
	Shader* const shader = material->GetShader();

	Batches& batches = material->GetFlags() == SURFACE_TRANSPARENT ? transparentBatches : opaqueBatches;
	Batch& batch = batches[shader][material];

	auto& list = c->IsDynamic() ? batch.dynamicList : batch.staticList;

	if (c->IsActive()) {
		Helpers::Insert(list, c);
	} else {
		Helpers::Remove(list, c);
	}
}

void VoxelRenderer::DynamicHandler(Events::Event * event) {
	VoxelRender* const c = static_cast<Events::AnyType<VoxelRender*>*>(event)->data;

	if (!c->IsActive()) return;

	Material::Base* const material = c->GetMaterial() ? c->GetMaterial() : defaultMaterial;
	Shader* const shader = material->GetShader();

	Batches& batches = material->GetFlags() == SURFACE_TRANSPARENT ? transparentBatches : opaqueBatches;
	Batch& batch = batches[shader][material];

	auto& previousList = c->IsDynamic() ? batch.staticList : batch.dynamicList;
	auto& currentList = c->IsDynamic() ? batch.dynamicList : batch.staticList;

	if (Helpers::Remove(previousList, c)) {
		Helpers::Insert(currentList, c);
		updateStatic = true;
	}
}

void VoxelRenderer::MaterialHandler(Events::Event * event) {
	const auto changeEvent = static_cast<Events::MaterialChange*>(event);
	auto c = static_cast<VoxelRender*>(changeEvent->component);

	if (!c->IsActive()) return;

	Material::Base* const material = c->GetMaterial() ? c->GetMaterial() : defaultMaterial;
	Shader* const shader = material->GetShader();

	Material::Base* const previousMaterial = changeEvent->previous ? changeEvent->previous : defaultMaterial;

	Batches& previousBatches = previousMaterial->GetFlags() == SURFACE_TRANSPARENT ? transparentBatches : opaqueBatches;
	Batches& currentBatches = material->GetFlags() == SURFACE_TRANSPARENT ? transparentBatches : opaqueBatches;

	Batch& previous = previousBatches[previousMaterial->GetShader()][previousMaterial];
	Batch& current = currentBatches[shader][material];

	if (c->IsDynamic()) {
		if (Helpers::Remove(previous.dynamicList, c)) {
			current.dynamicList.push_back(c);
		}
	} else {
		if (Helpers::Remove(previous.staticList, c)) {
			current.staticList.push_back(c);
			updateStatic = true;
		}
	}
}

void VoxelRenderer::ShaderHandler(Events::Event * event) {
	const auto changeEvent = static_cast<Events::ShaderChange*>(event);
	Material::Base* const material = changeEvent->material;

	Shader* const previous = changeEvent->previous;
	Shader* const current = material->GetShader();

	Batches& batches = material->GetFlags() == SURFACE_TRANSPARENT ? transparentBatches : opaqueBatches;
	batches[current][material] = batches[previous][material];
	batches[previous].erase(material);
	updateStatic = true;
}

