#include "StandardRenderer.h"

#include "Transform.h"

#include <Helpers/VectorHelpers.h>
#include <Events/EventsManager.h>
#include <GL/glew.h>

unsigned StandardRenderer::instanceBuffer = 0;
unsigned StandardRenderer::quadVAO = 0;

StandardRenderer::~StandardRenderer() {
	delete shader;
}

void StandardRenderer::Initialize(EntityManager * const manager) {
	Renderer::Initialize(manager);

	if (instanceBuffer == 0)
		glGenBuffers(1, &instanceBuffer);

	if (quadVAO == 0)
		GenerateQuad();

	shader = new Shader("Files/Shaders/standard2D.vert", "Files/Shaders/standard2D.frag");
	shader->Use();
	shader->SetInt("tex", 0);

	Events::EventsManager::GetInstance()->Subscribe("RENDER_ACTIVE", &StandardRenderer::ActiveHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("TEXTURE_CHANGE", &StandardRenderer::TextureChangeHandler, this);
}

void StandardRenderer::Render(Camera * const camera) {
	const auto& projection = camera->GetProjectionMatrix();
	const auto& lookAt = entities->GetComponent<Transform>(camera->entity)->GetLocalLookAt();

	shader->Use();
	shader->SetMatrix4("projection", projection);
	shader->SetMatrix4("view", lookAt);

	glBindVertexArray(quadVAO);

	for (auto& batchPair : batches) {
		auto batch = batchPair.second;

		if (batch.empty()) continue;

		unsigned count = batch.size();

		for (unsigned i = 0; i < count; ++i) {
			auto& instance = batch[i];
			auto& c = instance.component;

			auto transform = entities->GetComponent<Transform>(c->entity);

			instance.uvRect = c->uvRect;
			instance.tint = c->tint;
			instance.model = transform->GetWorldTransform();
		}

		count = batch.size();
		const unsigned unit = 4 * sizeof(float);
		const unsigned stride = sizeof(Instance);

		glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
		glBufferData(GL_ARRAY_BUFFER, count * stride, &batch[0], GL_STATIC_DRAW);

		unsigned i, offset;
		i = INSTANCE_LAYOUT_LOCATION;
		offset = sizeof(::Render*);

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

void StandardRenderer::ActiveHandler(Events::Event * event) {
	auto& c = static_cast<Events::AnyType<::Render*>*>(event)->data;
	auto& list = batches[c->GetTexture()];

	if (c->IsActive()) {
		Helpers::Insert(list, { 
			c,
			c->uvRect,
			c->tint
		});
	} else {
		Helpers::Remove(list, { c });
	}
}

void StandardRenderer::TextureChangeHandler(Events::Event* event) {
	auto changeEvent = static_cast<Events::TextureChange*>(event);
	auto& c = changeEvent->component;
	
	if (Helpers::Remove(batches[changeEvent->previous], { c })) {
		batches[c->GetTexture()].push_back({
			c,
			c->uvRect,
			c->tint
		});
	}
}

void StandardRenderer::GenerateQuad() {
	float quadVertices[] = {
		-0.5f,  0.5f,
		-0.5f, -0.5f,
		 0.5f, -0.5f,

		-0.5f,  0.5f,
		 0.5f, -0.5f,
		 0.5f,  0.5f
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

