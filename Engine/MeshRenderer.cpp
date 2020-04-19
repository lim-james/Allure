#include "MeshRenderer.h"

#include "Transform.h"

#include <Helpers/VectorHelpers.h>
#include <Events/EventsManager.h>
#include <GL/glew.h>

MeshRenderer::Batch::StaticData::StaticData() : VAO(0), count(0) { }

MeshRenderer::~MeshRenderer() {
	 delete defaultMaterial;
}

void MeshRenderer::Initialize(EntityManager * const manager) {
	Renderer::Initialize(manager);

	 defaultMaterial = new Material::MeshDefault;

	Events::EventsManager::GetInstance()->Subscribe("MESH_RENDER_ACTIVE", &MeshRenderer::ActiveHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("MESH_RENDER_DYNAMIC", &MeshRenderer::DynamicHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("MODEL_CHANGE", &MeshRenderer::ModelChangeHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("MESH_MATERIAL", &MeshRenderer::MaterialHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("MATERIAL_SHADER", &MeshRenderer::ShaderHandler, this);
}

void MeshRenderer::Render(RendererData const& data) {
	RenderBatches(data, opaqueBatches);
	RenderBatches(data, transparentBatches);
	updateStatic = false;
}

void MeshRenderer::InitializeInstanceBuffer(unsigned const& VAO, unsigned& instanceBuffer) {
	if (instanceBuffer) return;

	glBindVertexArray(VAO);

	glGenBuffers(1, &instanceBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);

	const unsigned unit = 4 * sizeof(float);
	const unsigned stride = sizeof(mat4f);
	unsigned i = INSTANCE_LAYOUT_LOCATION;

	for (unsigned u = 0; u < 4; ++u) {
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i++, 4, GL_FLOAT, GL_FALSE, stride, (void*)(u * unit));
	}

	for (; i >= INSTANCE_LAYOUT_LOCATION; --i)
		glVertexAttribDivisor(i, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MeshRenderer::RenderBatches(RendererData const& data, Batches& batches) {
	for (auto& shaderPair : batches) {
		Shader* const shader = shaderPair.first;

		shader->Use();
		shader->SetMatrix4("projection", data.projection);
		shader->SetMatrix4("view", data.view);

		for (auto& materialPair : shaderPair.second) {
			
			materialPair.first->SetAttributes();

			for (auto& batchPair : materialPair.second) {
				RenderStatic(data, batchPair.first, batchPair.second);
				RenderDynamic(data, batchPair.first, batchPair.second);
			}
		}
	}
}

void MeshRenderer::RenderStatic(RendererData const & data, Mesh* const mesh, Batch& batch) {
	Batch::StaticData& staticData = batch.staticData[data.camera];

	if (updateStatic) {
		if (batch.staticList.empty()) return;

		std::vector<mat4f> instances;
		instances.reserve(batch.staticList.size());

		for (MeshRender* const c : batch.staticList) {
			if (entities->GetLayer(c->entity) != data.camera->cullingMask) {
				continue;
			}

			Transform* const transform = entities->GetComponent<Transform>(c->entity);
			instances.push_back(transform->GetWorldTransform());
		}

		if (instances.empty()) return;

		if (staticData.VAO == 0)
			staticData.VAO = mesh->GenerateVAO();

		unsigned VBO = 0;
		InitializeInstanceBuffer(staticData.VAO, VBO);
		staticData.count = instances.size();

		glBindVertexArray(staticData.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, staticData.count * sizeof(mat4f), &instances[0], GL_STATIC_DRAW);
		glDrawElementsInstanced(GL_TRIANGLES, mesh->indicesSize, GL_UNSIGNED_INT, (void*)(0), staticData.count);
	} else {
		glBindVertexArray(staticData.VAO);
		glDrawElementsInstanced(GL_TRIANGLES, mesh->indicesSize, GL_UNSIGNED_INT, (void*)(0), staticData.count);
	}
}

void MeshRenderer::RenderDynamic(RendererData const& data, Mesh* const mesh, Batch const& batch) {
	if (batch.dynamicList.empty()) return;

	std::vector<mat4f> instances;
	instances.reserve(batch.dynamicList.size());

	for (MeshRender* const c : batch.dynamicList) {
		if (entities->GetLayer(c->entity) != data.camera->cullingMask) {
			continue;
		}

		Transform* const transform = entities->GetComponent<Transform>(c->entity);
		instances.push_back(transform->GetWorldTransform());
	}

	if (instances.empty()) return;

	const unsigned count = instances.size();
	glBindVertexArray(mesh->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, dynamicBuffers[mesh]);
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(mat4f), &instances[0], GL_STATIC_DRAW);
	glDrawElementsInstanced(GL_TRIANGLES, mesh->indicesSize, GL_UNSIGNED_INT, (void*)(0), count);
}

void MeshRenderer::ActiveHandler(Events::Event * event) {
	MeshRender* const c = static_cast<Events::AnyType<MeshRender*>*>(event)->data;

	Model* const model = c->GetModel();
	if (!model) return;

	Material::Base * const material = c->GetMaterial() ? c->GetMaterial() : defaultMaterial;
	Shader * const shader = material->GetShader();

	Batches& batches = material->GetFlags() == SURFACE_TRANSPARENT ? transparentBatches : opaqueBatches;
	MeshBatches& meshBatches = batches[shader][material];

	for (Mesh* const mesh : model->meshes) {
		Batch& batch = meshBatches[mesh];
		auto& list = c->IsDynamic() ? batch.dynamicList : batch.staticList;
		if (!c->IsDynamic()) updateStatic = true;

		if (c->IsActive()) {
			Helpers::Insert(list, c);

			if (dynamicBuffers.find(mesh) == dynamicBuffers.end()) {
				InitializeInstanceBuffer(mesh->VAO, dynamicBuffers[mesh]);
			}
		} else {
			Helpers::Remove(list, c);
		}
	}
}

void MeshRenderer::DynamicHandler(Events::Event * event) {
	MeshRender* const c = static_cast<Events::AnyType<MeshRender*>*>(event)->data;

	if (!c->IsActive()) return;

	Model* const model = c->GetModel();
	if (!model) return;

	Material::Base* const material = c->GetMaterial() ? c->GetMaterial() : defaultMaterial;
	Shader* const shader = material->GetShader();
	
	Batches& batches = material->GetFlags() == SURFACE_TRANSPARENT ? transparentBatches : opaqueBatches;
	MeshBatches& meshBatches = batches[shader][material];
	
	for (Mesh* const mesh : model->meshes) {
		Batch& batch = meshBatches[mesh];

		auto& previousList = c->IsDynamic() ? batch.staticList : batch.dynamicList;
		auto& currentList = c->IsDynamic() ? batch.dynamicList : batch.staticList;

		if (Helpers::Remove(previousList, c)) {
			Helpers::Insert(currentList, c);
			updateStatic = true;
		}
	}
}

void MeshRenderer::ModelChangeHandler(Events::Event* event) {
	const auto changeEvent = static_cast<Events::ModelChange*>(event);
	MeshRender* const c = changeEvent->component;

	if (!c->IsActive()) return;

	Material::Base* const material = c->GetMaterial() ? c->GetMaterial() : defaultMaterial;
	Shader* const shader = material->GetShader();

	Batches& batches = material->GetFlags() == SURFACE_TRANSPARENT ? transparentBatches : opaqueBatches;
	MeshBatches& meshBatches = batches[shader][material];

	Model* const current = c->GetModel();

	if (c->IsDynamic()) {
		if (changeEvent->previous) {
			for (Mesh* const mesh : changeEvent->previous->meshes) 
				Helpers::Remove(meshBatches[mesh].dynamicList, c);
		}

		if (current) {
			for (Mesh* const mesh : c->GetModel()->meshes) {
				Helpers::Insert(meshBatches[mesh].dynamicList, c);

				if (dynamicBuffers.find(mesh) == dynamicBuffers.end()) {
					InitializeInstanceBuffer(mesh->VAO, dynamicBuffers[mesh]);
				}
			}
		}
	} else {
		if (changeEvent->previous) {
			for (Mesh* const mesh : changeEvent->previous->meshes) 
				Helpers::Remove(meshBatches[mesh].staticList, c);
		}

		if (current) {
			for (Mesh* const mesh : c->GetModel()->meshes) {
				Helpers::Insert(meshBatches[mesh].staticList, c);

				if (dynamicBuffers.find(mesh) == dynamicBuffers.end()) {
					InitializeInstanceBuffer(mesh->VAO, dynamicBuffers[mesh]);
				}
			}
		}
			
		updateStatic = true;
	}
}

void MeshRenderer::MaterialHandler(Events::Event * event) {
	const auto changeEvent = static_cast<Events::MaterialChange*>(event);
	MeshRender* const c = static_cast<MeshRender*>(changeEvent->component);

	if (!c->IsActive()) return;

	Model* const model = c->GetModel();
	if (!model) return;

	Material::Base* const material = c->GetMaterial() ? c->GetMaterial() : defaultMaterial;
	Shader* const shader = material->GetShader();

	Material::Base* previousMaterial = changeEvent->previous ? changeEvent->previous : defaultMaterial;

	Batches& previousBatches = previousMaterial->GetFlags() == SURFACE_TRANSPARENT ? transparentBatches : opaqueBatches;
	Batches& currentBatches = material->GetFlags() == SURFACE_TRANSPARENT ? transparentBatches : opaqueBatches;

	auto& previous = previousBatches[previousMaterial->GetShader()][previousMaterial];
	auto& current = currentBatches[shader][material];

	for (Mesh* const mesh : model->meshes) {
		if (c->IsDynamic()) {
			if (Helpers::Remove(previous[mesh].dynamicList, c)) {
				Helpers::Insert(current[mesh].dynamicList, c);
			}
		} else {
			if (Helpers::Remove(previous[mesh].staticList, c)) {
				Helpers::Insert(current[mesh].staticList, c);
				updateStatic = true;
			}
		}
	}
}

void MeshRenderer::ShaderHandler(Events::Event * event) {
	const auto changeEvent = static_cast<Events::ShaderChange*>(event);
	Material::Base* const material = changeEvent->material;
	
	Shader* const previous = changeEvent->previous;
	Shader* const current = material->GetShader();

	Batches& batches = material->GetFlags() == SURFACE_TRANSPARENT ? transparentBatches : opaqueBatches;
	batches[current][material] = batches[previous][material];
	batches[previous].erase(material);
	updateStatic = true;
}

