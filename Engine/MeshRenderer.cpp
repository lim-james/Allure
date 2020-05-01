#include "MeshRenderer.h"

#include "Transform.h"

#include <Helpers/VectorHelpers.h>
#include <Events/EventsManager.h>
#include <GL/glew.h>

MeshRenderer::Batch::StaticData::StaticData() : VAO(0), count(0) { }

MeshRenderer::~MeshRenderer() {
	delete depthShader;
	delete defaultMaterial;
}

void MeshRenderer::Initialize(EntityManager * const manager) {
	Renderer::Initialize(manager);

	depthShader = new Shader("Files/Shaders/depth3D.vert", "Files/Shaders/depth3D.frag");
	defaultMaterial = new Material::MeshDefault;

	EventsManager* const em = EventsManager::Get();
	em->Subscribe("MESH_RENDER_ACTIVE", &MeshRenderer::ActiveHandler, this);
	em->Subscribe("MESH_RENDER_DYNAMIC", &MeshRenderer::DynamicHandler, this);
	em->Subscribe("MESH_CHANGE", &MeshRenderer::ModelChangeHandler, this);
	em->Subscribe("MESH_MATERIAL", &MeshRenderer::MaterialHandler, this);
	em->Subscribe("MATERIAL_SHADER", &MeshRenderer::ShaderHandler, this);
	em->Subscribe("CAST_SHADOW", &MeshRenderer::CastShadowHandler, this);
}

void MeshRenderer::RenderDepth(RendererData const& data) {
	RenderDepthBatches(data, opaqueBatches);
	RenderDepthBatches(data, transparentBatches);
}

void MeshRenderer::RenderOpaque(RendererData const& data) {
	RenderBatches(data, opaqueBatches);
}

void MeshRenderer::RenderTransparent(RendererData const& data) {
	RenderBatches(data, transparentBatches);
}

void MeshRenderer::PostRender() {
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

void MeshRenderer::RenderDepthBatches(RendererData const & data, Batches & batches) {
	const unsigned lightCount = data.lights ? data.lights->size() : 0;

	depthShader->Use();
	depthShader->SetMatrix4("projection", data.projection);
	depthShader->SetMatrix4("view", data.view);

	for (auto& shaderPair : batches) {
		for (auto& materialPair : shaderPair.second) {
			for (auto& batchPair : materialPair.second) {
				Mesh* const mesh = batchPair.first;
				depthShader->SetMatrix4("model", mesh->modelTransform);
				RenderStatic(data, mesh, batchPair.second);
				RenderDynamic(data, mesh, batchPair.second);
			}
		}
	}

}

void MeshRenderer::RenderBatches(RendererData const& data, Batches& batches) {
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

			for (auto& batchPair : materialPair.second) {
				Mesh* const mesh = batchPair.first;
				shader->SetMatrix4("model", mesh->modelTransform);
				RenderStatic(data, mesh, batchPair.second);
				RenderDynamic(data, mesh, batchPair.second);
			}
		}
	}
}

void MeshRenderer::RenderStatic(RendererData const & data, Mesh* const mesh, Batch& batch) {
	if (updateStatic || batch.staticData.find(data.object) == batch.staticData.end()) {
		if (batch.staticList.empty()) return;

		std::vector<mat4f> instances;
		instances.reserve(batch.staticList.size());

		for (MeshRender* const c : batch.staticList) {
			if (entities->GetLayer(c->entity) != data.cullingMask) {
				continue;
			}

			Transform* const transform = entities->GetComponent<Transform>(c->entity);
			instances.push_back(transform->GetWorldTransform());
		}

		if (instances.empty()) return;

		Batch::StaticData& staticData = batch.staticData[data.object];

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
		Batch::StaticData& staticData = batch.staticData.at(data.object);
		glBindVertexArray(staticData.VAO);
		glDrawElementsInstanced(GL_TRIANGLES, mesh->indicesSize, GL_UNSIGNED_INT, (void*)(0), staticData.count);
	}
}

void MeshRenderer::RenderDynamic(RendererData const& data, Mesh* const mesh, Batch const& batch) {
	if (batch.dynamicList.empty()) return;

	std::vector<mat4f> instances;
	instances.reserve(batch.dynamicList.size());

	for (MeshRender* const c : batch.dynamicList) {
		if (entities->GetLayer(c->entity) != data.cullingMask) {
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

	Mesh* const mesh = c->GetMesh();
	if (!mesh) return;

	Material::Base * const material = c->GetMaterial() ? c->GetMaterial() : defaultMaterial;
	Shader * const shader = material->GetShader();

	Batches& batches = material->GetFlags() == SURFACE_TRANSPARENT ? transparentBatches : opaqueBatches;
	Batch& batch = batches[shader][material][mesh];
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

void MeshRenderer::DynamicHandler(Events::Event * event) {
	MeshRender* const c = static_cast<Events::AnyType<MeshRender*>*>(event)->data;

	if (!c->IsActive()) return;

	Mesh* const mesh = c->GetMesh();
	if (!mesh) return;

	Material::Base* const material = c->GetMaterial() ? c->GetMaterial() : defaultMaterial;
	Shader* const shader = material->GetShader();

	Batches& batches = material->GetFlags() == SURFACE_TRANSPARENT ? transparentBatches : opaqueBatches;
	Batch& batch = batches[shader][material][mesh];

	auto& previousList = c->IsDynamic() ? batch.staticList : batch.dynamicList;
	auto& currentList = c->IsDynamic() ? batch.dynamicList : batch.staticList;

	if (Helpers::Remove(previousList, c)) {
		Helpers::Insert(currentList, c);
		updateStatic = true;
	}
}

void MeshRenderer::ModelChangeHandler(Events::Event* event) {
	const auto changeEvent = static_cast<Events::MeshChange*>(event);
	MeshRender* const c = changeEvent->component;

	if (!c->IsActive()) return;

	Material::Base* const material = c->GetMaterial() ? c->GetMaterial() : defaultMaterial;
	Shader* const shader = material->GetShader();

	Batches& batches = material->GetFlags() == SURFACE_TRANSPARENT ? transparentBatches : opaqueBatches;
	MeshBatches& meshBatches = batches[shader][material];
	Mesh* current = c->GetMesh();

	if (c->IsDynamic()) {
		if (changeEvent->previous) {
			Helpers::Remove(meshBatches[changeEvent->previous].dynamicList, c);
		}

		if (current) {
			Helpers::Insert(meshBatches[current].dynamicList, c);

			if (dynamicBuffers.find(current) == dynamicBuffers.end()) {
				InitializeInstanceBuffer(current->VAO, dynamicBuffers[current]);
			}
		}
	} else {
		if (changeEvent->previous) {
			Helpers::Remove(meshBatches[changeEvent->previous].staticList, c);
		}

		if (current) {
			Helpers::Insert(meshBatches[current].staticList, c);

			if (dynamicBuffers.find(current) == dynamicBuffers.end()) {
				InitializeInstanceBuffer(current->VAO, dynamicBuffers[current]);
			}
		}
			
		updateStatic = true;
	}
}

void MeshRenderer::MaterialHandler(Events::Event * event) {
	const auto changeEvent = static_cast<Events::MaterialChange*>(event);
	MeshRender* const c = static_cast<MeshRender*>(changeEvent->component);

	if (!c->IsActive()) return;

	Mesh* const mesh = c->GetMesh();
	if (!mesh) return;

	Material::Base* const material = c->GetMaterial() ? c->GetMaterial() : defaultMaterial;
	Shader* const shader = material->GetShader();

	Material::Base* previousMaterial = changeEvent->previous ? changeEvent->previous : defaultMaterial;

	Batches& previousBatches = previousMaterial->GetFlags() == SURFACE_TRANSPARENT ? transparentBatches : opaqueBatches;
	Batches& currentBatches = material->GetFlags() == SURFACE_TRANSPARENT ? transparentBatches : opaqueBatches;

	auto& previous = previousBatches[previousMaterial->GetShader()][previousMaterial][mesh];
	auto& current = currentBatches[shader][material][mesh];

	if (c->IsDynamic()) {
		if (Helpers::Remove(previous.dynamicList, c)) {
			Helpers::Insert(current.dynamicList, c);
		}
	} else {
		if (Helpers::Remove(previous.staticList, c)) {
			Helpers::Insert(current.staticList, c);
			updateStatic = true;
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

void MeshRenderer::CastShadowHandler(Events::Event * event) {
	MeshRender* const c = static_cast<Events::AnyType<MeshRender*>*>(event)->data;
	
}

