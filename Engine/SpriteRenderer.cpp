#include "SpriteRenderer.h"

#include "Transform.h"

#include <Helpers/VectorHelpers.h>
#include <Events/EventsManager.h>
#include <GL/glew.h>

unsigned SpriteRenderer::dynamicVAO = 0;
unsigned SpriteRenderer::dynamicBuffer = 0;

SpriteRenderer::Batch::StaticData::StaticData() : VAO(0), count(0) { }

SpriteRenderer::~SpriteRenderer() {
	delete defaultMaterial;
}

void SpriteRenderer::Initialize(EntityManager * const manager) {
	Renderer::Initialize(manager);

	if (dynamicVAO == 0)
		GenerateQuad(dynamicVAO);

	InitializeInstanceBuffer(dynamicVAO, dynamicBuffer);

	defaultMaterial = new Material::SpriteDefault;

	EventsManager::Get()->Subscribe("SPRITE_RENDER_ACTIVE", &SpriteRenderer::ActiveHandler, this);
	EventsManager::Get()->Subscribe("SPRITE_RENDER_DYNAMIC", &SpriteRenderer::DynamicHandler, this);
	EventsManager::Get()->Subscribe("SPRITE_CHANGE", &SpriteRenderer::SpriteChangeHandler, this);
	EventsManager::Get()->Subscribe("SPRITE_MATERIAL", &SpriteRenderer::MaterialHandler, this);
	EventsManager::Get()->Subscribe("MATERIAL_SHADER", &SpriteRenderer::ShaderHandler, this);
}

void SpriteRenderer::RenderDepth(RendererData const& data) {
	RenderBatches(data, opaqueBatches);
	RenderBatches(data, transparentBatches);
}

void SpriteRenderer::RenderOpaque(RendererData const & data) {
	RenderBatches(data, opaqueBatches);
}

void SpriteRenderer::RenderTransparent(RendererData const & data) {
	RenderBatches(data, transparentBatches);
}

void SpriteRenderer::PostRender() {
	updateStatic = false;
}

void SpriteRenderer::GenerateQuad(unsigned& VAO) {
	const float quadVertices[] = {
		-0.5f, 0.5f,
		-0.5f, -0.5f,
		 0.5f, -0.5f,

		-0.5f, 0.5f,
		 0.5f, -0.5f,
		 0.5f, 0.5f
	};

	unsigned VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vec2f), (void*)0);
}

void SpriteRenderer::InitializeInstanceBuffer(unsigned const& VAO, unsigned& instanceBuffer) {
	if (instanceBuffer) return;

	glBindVertexArray(VAO);

	glGenBuffers(1, &instanceBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);

	const unsigned unit = 4 * sizeof(float);
	const unsigned stride = sizeof(Instance);

	unsigned i = INSTANCE_LAYOUT_LOCATION;
	unsigned offset = 0;

	// rect	
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i++, 4, GL_FLOAT, GL_FALSE, stride, (void*)offset);
	offset += sizeof(vec4f);
	// tint	
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i++, 4, GL_FLOAT, GL_FALSE, stride, (void*)offset);
	offset += sizeof(vec4f);
	// model
	for (unsigned u = 0; u < 4; ++u) {
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i++, 4, GL_FLOAT, GL_FALSE, stride, (void*)(u * unit + offset));
	}

	for (; i >= INSTANCE_LAYOUT_LOCATION; --i)
		glVertexAttribDivisor(i, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SpriteRenderer::RenderBatches(RendererData const& data, Batches& batches) {
	for (auto& shaderPair : batches) {
		Shader* const shader = shaderPair.first;

		shader->Use();
		shader->SetMatrix4("projection", data.projection);
		shader->SetMatrix4("view", data.view);

		for (auto& materialPair : shaderPair.second) {
			
			materialPair.first->Use();

			for (auto& batchPair : materialPair.second) {
				auto const& tex = batchPair.first;
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, tex);
				shader->SetInt("useTex", tex);

				RenderStatic(data, batchPair.second);
				RenderDynamic(data, batchPair.second);
			}
		}
	}
}

void SpriteRenderer::RenderStatic(RendererData const & data, Batch& batch) {
	if (updateStatic || batch.staticData.find(data.camera) == batch.staticData.end()) {
		if (batch.staticList.empty()) return;

		std::vector<Instance> instances;
		instances.reserve(batch.staticList.size());

		for (SpriteRender* const c : batch.staticList) {
			if (entities->GetLayer(c->entity) != data.camera->cullingMask) {
				continue;
			}

			Transform* const transform = entities->GetComponent<Transform>(c->entity);

			Instance instance;
			instance.uvRect = c->uvRect;
			instance.tint = c->tint;
			instance.model = transform->GetWorldTransform();
			instances.push_back(instance);
		}

		if (instances.empty()) return;

		Batch::StaticData& staticData = batch.staticData[data.camera];

		if (staticData.VAO == 0)
			GenerateQuad(staticData.VAO);

		unsigned VBO = 0;
		InitializeInstanceBuffer(staticData.VAO, VBO);
		staticData.count = instances.size();

		glBindVertexArray(staticData.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, staticData.count * sizeof(Instance), &instances[0], GL_STATIC_DRAW);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, staticData.count);
	} else {
		Batch::StaticData& staticData = batch.staticData[data.camera];
		glBindVertexArray(staticData.VAO);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, staticData.count);
	}

}

void SpriteRenderer::RenderDynamic(RendererData const& data, Batch const& batch) {
	if (batch.dynamicList.empty()) return;

	std::vector<Instance> instances;
	instances.reserve(batch.dynamicList.size());

	for (SpriteRender* const c : batch.dynamicList) {
		if (entities->GetLayer(c->entity) != data.camera->cullingMask) {
			continue;
		}

		Transform* const transform = entities->GetComponent<Transform>(c->entity);

		Instance instance;
		instance.uvRect = c->uvRect;
		instance.tint = c->tint;
		instance.model = transform->GetWorldTransform();
		instances.push_back(instance);
	}

	if (instances.empty()) return;

	const unsigned count = instances.size();
	glBindVertexArray(dynamicVAO);
	glBindBuffer(GL_ARRAY_BUFFER, dynamicBuffer);
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(Instance), &instances[0], GL_STATIC_DRAW);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, count);
}

void SpriteRenderer::ActiveHandler(Events::Event * event) {
	SpriteRender* const c = static_cast<Events::AnyType<SpriteRender*>*>(event)->data;

	if (!c->IsDynamic()) updateStatic = true;

	Material::Base* const material = c->GetMaterial() ? c->GetMaterial() : defaultMaterial;
	Shader* const shader = material->GetShader();

	Batches& batches = material->GetFlags() == SURFACE_TRANSPARENT ? transparentBatches : opaqueBatches;
	Batch& batch = batches[shader][material][c->GetSprite()];

	auto& list = c->IsDynamic() ? batch.dynamicList : batch.staticList;

	if (c->IsActive()) {
		Helpers::Insert(list, c);
	} else {
		Helpers::Remove(list, c);
	}
}

void SpriteRenderer::DynamicHandler(Events::Event * event) {
	SpriteRender* const c = static_cast<Events::AnyType<SpriteRender*>*>(event)->data;

	if (!c->IsActive()) return;

	Material::Base* const material = c->GetMaterial() ? c->GetMaterial() : defaultMaterial;
	Shader* const shader = material->GetShader();
	unsigned const& sprite = c->GetSprite();

	Batches& batches = material->GetFlags() == SURFACE_TRANSPARENT ? transparentBatches : opaqueBatches;
	Batch& batch = batches[shader][material][sprite];

	auto& previousList = c->IsDynamic() ? batch.staticList : batch.dynamicList;
	auto& currentList = c->IsDynamic() ? batch.dynamicList : batch.staticList;

	if (Helpers::Remove(previousList, c)) {
		Helpers::Insert(currentList, c);
		updateStatic = true;
	}
}

void SpriteRenderer::SpriteChangeHandler(Events::Event* event) {
	const auto changeEvent = static_cast<Events::SpriteChange*>(event);
	SpriteRender* const c = changeEvent->component;

	if (!c->IsActive()) return;

	Material::Base* const material = c->GetMaterial() ? c->GetMaterial() : defaultMaterial;
	Shader* const shader = material->GetShader();
	
	Batches& batches = material->GetFlags() == SURFACE_TRANSPARENT ? transparentBatches : opaqueBatches;
	SpriteBatches& batch = batches[shader][material];

	Batch& previous = batch[changeEvent->previous];
	Batch& current = batch[c->GetSprite()];

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

void SpriteRenderer::MaterialHandler(Events::Event * event) {
	const auto changeEvent = static_cast<Events::MaterialChange*>(event);
	auto c = static_cast<SpriteRender*>(changeEvent->component);

	if (!c->IsActive()) return;

	unsigned const& sprite = c->GetSprite();
	Material::Base* const material = c->GetMaterial() ? c->GetMaterial() : defaultMaterial;
	Shader* const shader = material->GetShader();

	Material::Base* const previousMaterial = changeEvent->previous ? changeEvent->previous : defaultMaterial;

	Batches& previousBatches = previousMaterial->GetFlags() == SURFACE_TRANSPARENT ? transparentBatches : opaqueBatches;
	Batches& currentBatches = material->GetFlags() == SURFACE_TRANSPARENT ? transparentBatches : opaqueBatches;

	Batch& previous = previousBatches[previousMaterial->GetShader()][previousMaterial][sprite];
	Batch& current = currentBatches[shader][material][sprite];

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

void SpriteRenderer::ShaderHandler(Events::Event * event) {
	const auto changeEvent = static_cast<Events::ShaderChange*>(event);
	Material::Base* const material = changeEvent->material;
	
	Shader* const previous = changeEvent->previous;
	Shader* const current = material->GetShader();

	Batches& batches = material->GetFlags() == SURFACE_TRANSPARENT ? transparentBatches : opaqueBatches;
	batches[current][material] = batches[previous][material];
	batches[previous].erase(material);
	updateStatic = true;
}
