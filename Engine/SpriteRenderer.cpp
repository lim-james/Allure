#include "SpriteRenderer.h"

#include "Transform.h"

#include <Helpers/VectorHelpers.h>
#include <Events/EventsManager.h>
#include <GL/glew.h>

unsigned SpriteRenderer::instanceBuffer = 0;
unsigned SpriteRenderer::quadVAO = 0;

SpriteRenderer::~SpriteRenderer() {
	delete shader;
}

void SpriteRenderer::Initialize(EntityManager * const manager) {
	Renderer::Initialize(manager);

	if (instanceBuffer == 0)
		glGenBuffers(1, &instanceBuffer);

	if (quadVAO == 0)
		GenerateQuad();

	shader = new Shader("Files/Shaders/standard2D.vert", "Files/Shaders/standard2D.frag");
	shader->Use();
	shader->SetInt("tex", 0);

	Events::EventsManager::GetInstance()->Subscribe("SPRITE_RENDER_ACTIVE", &SpriteRenderer::ActiveHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("TEXTURE_CHANGE", &SpriteRenderer::SpriteChangeHandler, this);
}

void SpriteRenderer::Render(RendererData const& data) {
	shader->Use();
	shader->SetMatrix4("projection", data.projection);
	shader->SetMatrix4("view", data.view);

	glBindVertexArray(quadVAO);

	for (auto& batchPair : batches) {
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

		const unsigned count = instances.size();
		const unsigned unit = 4 * sizeof(float);
		const unsigned stride = sizeof(Instance);

		glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
		glBufferData(GL_ARRAY_BUFFER, count * stride, &instances[0], GL_STATIC_DRAW);

		unsigned i = INSTANCE_LAYOUT_LOCATION;
		unsigned offset = 0;

		// rect	
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i++, 4, GL_FLOAT, GL_FALSE, stride, (void*)(offset));
		offset += sizeof(vec4f);
		// tint	
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i++, 4, GL_FLOAT, GL_FALSE, stride, (void*)(offset));
		offset += sizeof(vec4f);
		// model
		for (int u = 0; u < 4; ++u) {
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i++, 4, GL_FLOAT, GL_FALSE, stride, (void*)(u * unit + offset));
		}

		for (; i >= INSTANCE_LAYOUT_LOCATION; --i)
			glVertexAttribDivisor(i, 1);

		const auto tex = batchPair.first;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		shader->SetInt("useTex", tex);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, count);
	}
}

void SpriteRenderer::ActiveHandler(Events::Event * event) {
	auto& c = static_cast<Events::AnyType<::SpriteRender*>*>(event)->data;
	auto& list = batches[c->GetSprite()];

	if (c->IsActive()) {
		Helpers::Insert(list, c);
	} else {
		Helpers::Remove(list, c);
	}
}

void SpriteRenderer::SpriteChangeHandler(Events::Event* event) {
	auto changeEvent = static_cast<Events::SpriteChange*>(event);
	auto& c = changeEvent->component;
	
	if (Helpers::Remove(batches[changeEvent->previous], c)) {
		batches[c->GetSprite()].push_back(c);
	}
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

