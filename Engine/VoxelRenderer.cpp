#include "VoxelRenderer.h"

#include "Transform.h"
#include "LoadModel.h"

#include <Helpers/VectorHelpers.h>
#include <Events/EventsManager.h>
#include <GL/glew.h>

Mesh* VoxelRenderer::cube = nullptr;
unsigned VoxelRenderer::dynamicVAO = 0;
unsigned VoxelRenderer::dynamicBuffer = 0;

VoxelRenderer::Batch::StaticData::StaticData() : VAO(0), count(0) { }

VoxelRenderer::~VoxelRenderer() {
	delete depthShader;
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

	depthShader = new Shader("Files/Shaders/depthVX.vert", "Files/Shaders/depthVX.frag");
	defaultMaterial = new Material::VoxelDefault;

	EventsManager::Get()->Subscribe("VOXEL_RENDER_ACTIVE", &VoxelRenderer::ActiveHandler, this);
	EventsManager::Get()->Subscribe("VOXEL_RENDER_DYNAMIC", &VoxelRenderer::DynamicHandler, this);
	EventsManager::Get()->Subscribe("VOXEL_MATERIAL", &VoxelRenderer::MaterialHandler, this);
	EventsManager::Get()->Subscribe("MATERIAL_SHADER", &VoxelRenderer::ShaderHandler, this);
}

void VoxelRenderer::RenderDepth(RendererData const& data) {
	RenderDepthBatches(data, opaqueBatches);
	RenderDepthBatches(data, transparentBatches);
}

void VoxelRenderer::RenderOpaque(RendererData const & data) {
	RenderBatches(data, opaqueBatches);
}

void VoxelRenderer::RenderTransparent(RendererData const & data) {
	RenderBatches(data, transparentBatches);
}

void VoxelRenderer::PostRender() {
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

void VoxelRenderer::RenderDepthBatches(RendererData const & data, Batches & batches) {
	depthShader->Use();
	depthShader->SetMatrix4("projection", data.projection);
	depthShader->SetMatrix4("view", data.view);

	for (auto& shaderPair : batches) {
		for (auto& materialPair : shaderPair.second) {
			RenderStatic(data, materialPair.second);
			RenderDynamic(data, materialPair.second);
		}
	}
}

void VoxelRenderer::RenderBatches(RendererData const& data, Batches& batches) {
	const unsigned lightCount = data.lights ? data.lights->size() : 0;

	for (auto& shaderPair : batches) {
		Shader* const shader = shaderPair.first;

		shader->Use();
		shader->SetMatrix4("projection", data.projection);
		shader->SetMatrix4("view", data.view);

		shader->SetVector3("viewPosition", data.viewPosition);
		shader->SetInt("lightCount", lightCount);

		for (unsigned i = 0; i < lightCount; ++i) {
			Light* const light = data.lights->at(i);
			const std::string tag = "lights[" + std::to_string(i) + "].";

			shader->SetInt(tag + "type", light->type);
			shader->SetFloat(tag + "range", light->range);
			shader->SetFloat(tag + "innerCutOff", cos(light->innerCutOff * Math::toRad));
			shader->SetFloat(tag + "outerCutOff", cos(light->outerCutOff * Math::toRad));
			shader->SetVector3(tag + "color", light->color);
			shader->SetFloat(tag + "intensity", light->intensity);
			shader->SetFloat(tag + "strength", light->strength);

			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, light->shadowMap);

			Transform* const transform = entities->GetComponent<Transform>(light->entity);
			shader->SetVector3(tag + "position", transform->GetWorldTranslation());
			shader->SetVector3(tag + "direction", transform->GetLocalFront());

			shader->SetMatrix4("lightSpaceMatrices[" + std::to_string(i) + ']', data.lightSpaceMatrices[i]);
		}

		for (auto& materialPair : shaderPair.second) {
			materialPair.first->Use();
			RenderStatic(data, materialPair.second);
			RenderDynamic(data, materialPair.second);
		}
	}
}

void VoxelRenderer::RenderStatic(RendererData const & data, Batch& batch) {
	if (updateStatic || batch.staticData.find(data.object) == batch.staticData.end()) {
		if (batch.staticList.empty()) return;

		std::vector<Instance> instances;
		instances.reserve(batch.staticList.size());

		for (VoxelRender* const c : batch.staticList) {
			if (entities->GetLayer(c->entity) != data.cullingMask) {
				continue;
			}

			Transform* const transform = entities->GetComponent<Transform>(c->entity);

			Instance instance;
			instance.tint = c->tint;
			instance.position = transform->GetWorldTranslation();
			instances.push_back(instance);
		}

		if (instances.empty()) return;

		Batch::StaticData& staticData = batch.staticData.at(data.object);

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
		Batch::StaticData& staticData = batch.staticData.at(data.object);
		glBindVertexArray(staticData.VAO);
		glDrawElementsInstanced(GL_TRIANGLES, cube->indicesSize, GL_UNSIGNED_INT, (void*)(0), staticData.count);
	}

}

void VoxelRenderer::RenderDynamic(RendererData const& data, Batch const& batch) {
	if (batch.dynamicList.empty()) return;

	std::vector<Instance> instances;
	instances.reserve(batch.dynamicList.size());

	for (VoxelRender* const c : batch.dynamicList) {
		if (entities->GetLayer(c->entity) != data.cullingMask) {
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

