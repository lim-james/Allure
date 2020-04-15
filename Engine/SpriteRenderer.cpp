#include "SpriteRenderer.h"

#include "Transform.h"

#include <Helpers/VectorHelpers.h>
#include <Events/EventsManager.h>
#include <GL/glew.h>

unsigned SpriteRenderer::instanceBuffer = 0;
unsigned SpriteRenderer::quadVAO = 0;

SpriteRenderer::~SpriteRenderer() {
	delete defaultMaterial;
}

void SpriteRenderer::Initialize(EntityManager * const manager) {
	Renderer::Initialize(manager);

	if (quadVAO == 0)
		GenerateQuad();

	if (instanceBuffer == 0) {
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
		for (int u = 0; u < 4; ++u) {
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i++, 4, GL_FLOAT, GL_FALSE, stride, (void*)(u * unit + offset));
		}

		for (; i >= INSTANCE_LAYOUT_LOCATION; --i)
			glVertexAttribDivisor(i, 1);
	}

	defaultMaterial = new Material::SpriteDefault;

	Events::EventsManager::GetInstance()->Subscribe("SPRITE_RENDER_ACTIVE", &SpriteRenderer::ActiveHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("SPRITE_CHANGE", &SpriteRenderer::SpriteChangeHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("SPRITE_MATERIAL", &SpriteRenderer::MaterialHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("MATERIAL_SHADER", &SpriteRenderer::ShaderHandler, this);
}

void SpriteRenderer::Render(RendererData const& data) {
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);

	for (auto& shaderPair : batches) {
	
		Shader* const shader = shaderPair.first;

		shader->Use();
		shader->SetMatrix4("projection", data.projection);
		shader->SetMatrix4("view", data.view);

		for (auto& materialPair : shaderPair.second) {
			
			materialPair.first->SetAttributes();

			for (auto& batchPair : materialPair.second) {
				auto batch = batchPair.second;

				if (batch.empty()) continue;

				std::vector<Instance> instances;
				instances.reserve(batch.size());

				for (auto& c : batch) {
					if (entities->GetLayer(c->entity) != data.camera->cullingMask) {
						continue;
					}

					auto transform = entities->GetComponent<Transform>(c->entity);

					Instance instance;
					instance.uvRect = c->uvRect;
					instance.tint = c->tint;
					instance.model = transform->GetWorldTransform();
					instances.push_back(instance);
				}

				if (instances.empty()) continue;

				const auto tex = batchPair.first;
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, tex);
				shader->SetInt("useTex", tex);

				const unsigned count = instances.size();
				glBufferData(GL_ARRAY_BUFFER, count * sizeof(Instance), &instances[0], GL_STATIC_DRAW);
				glDrawArraysInstanced(GL_TRIANGLES, 0, 6, count);
			}
		}
	}
}

void SpriteRenderer::ActiveHandler(Events::Event * event) {
	auto& c = static_cast<Events::AnyType<SpriteRender*>*>(event)->data;

	auto material = c->GetMaterial() ? c->GetMaterial() : defaultMaterial;
	auto shader = material->GetShader();
	auto& list = batches[shader][material][c->GetSprite()];

	if (c->IsActive()) {
		Helpers::Insert(list, c);
	} else {
		Helpers::Remove(list, c);
	}
}

void SpriteRenderer::SpriteChangeHandler(Events::Event* event) {
	auto changeEvent = static_cast<Events::SpriteChange*>(event);
	auto& c = changeEvent->component;

	auto material = c->GetMaterial() ? c->GetMaterial() : defaultMaterial;
	auto shader = material->GetShader();
	
	auto& batch = batches[shader][material];

	if (Helpers::Remove(batch[changeEvent->previous], c)) {
		batch[c->GetSprite()].push_back(c);
	}
}

void SpriteRenderer::MaterialHandler(Events::Event * event) {
	auto changeEvent = static_cast<Events::MaterialChange*>(event);
	auto c = static_cast<SpriteRender*>(changeEvent->component);

	auto const& sprite = c->GetSprite();
	auto material = c->GetMaterial() ? c->GetMaterial() : defaultMaterial;
	auto shader = material->GetShader();

	auto previousMaterial = changeEvent->previous ? changeEvent->previous : defaultMaterial;
	auto& previous = batches[previousMaterial->GetShader()][previousMaterial];
	auto& current = batches[shader][material];

	if (Helpers::Remove(previous[sprite], c)) {
		current[sprite].push_back(c);
	}
}

void SpriteRenderer::ShaderHandler(Events::Event * event) {
	auto changeEvent = static_cast<Events::ShaderChange*>(event);
	auto& material = changeEvent->material;
	
	auto previous = changeEvent->previous;
	auto current = material->GetShader();

	batches[current][material] = batches[previous][material];
	batches[previous].erase(material);
}

void SpriteRenderer::GenerateQuad() {
	float quadVertices[] = {
		-0.5f, 0.5f,
		-0.5f, -0.5f,
		 0.5f, -0.5f,

		-0.5f, 0.5f,
		 0.5f, -0.5f,
		 0.5f, 0.5f
	};

	unsigned VBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vec2f), (void*)0);
}


