#include "LineRenderer.h"

#include "Transform.h"

#include <Helpers/VectorHelpers.h>
#include <Events/EventsManager.h>
#include <GL/glew.h>

unsigned LineRenderer::instanceBuffer = 0;
unsigned LineRenderer::lineVAO = 0;

LineRenderer::~LineRenderer() {
	delete defaultMaterial;
}

void LineRenderer::Initialize(EntityManager * const manager) {
	Renderer::Initialize(manager);

	if (instanceBuffer == 0)
		glGenBuffers(1, &instanceBuffer);

	if (lineVAO == 0)
		GenerateLine();

	defaultMaterial = new Material::LineDefault;
	InitializeShader(defaultMaterial->GetShader());

	//Events::EventsManager::GetInstance()->Subscribe("DRAW_LINE", &LineRenderer::DrawLineHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("LINE_RENDER_ACTIVE", &LineRenderer::ActiveHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("LINE_MATERIAL", &LineRenderer::MaterialHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("MATERIAL_SHADER", &LineRenderer::ShaderHandler, this);
}

void LineRenderer::Render(RendererData const& data) {
	glDisable(GL_DEPTH_TEST);
	glLineWidth(data.camera->GetSize() * .25f);

	for (auto& shaderPair : batches) {

		Shader * const shader = shaderPair.first;

		shader->Use();
		shader->SetMatrix4("projection", data.projection);
		shader->SetMatrix4("view", data.view);

		glBindVertexArray(lineVAO);
		glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);

		for (auto& materialPair : shaderPair.second) {
			materialPair.first->SetAttributes();

			auto& batch = materialPair.second;
			if (batch.empty()) continue;

			std::vector<Instance> instances;
			instances.reserve(batch.size());

			for (auto& c : batch) {
				if (entities->GetLayer(c->entity) != data.camera->cullingMask) {
					continue;
				}

				auto transform = entities->GetComponent<Transform>(c->entity);

				Instance instance;
				instance.offset = c->offset;
				instance.length = c->length;
				instance.tint = c->tint;
				instance.model = transform->GetWorldTransform();
				instances.push_back(instance);
			}

			if (instances.empty()) continue;

			const unsigned count = instances.size();
			glBufferData(GL_ARRAY_BUFFER, count * sizeof(Instance), &instances[0], GL_STATIC_DRAW);
			glDrawArraysInstanced(GL_LINES, 0, 2, count);
		}
	}

	glEnable(GL_DEPTH_TEST);
}

void LineRenderer::ActiveHandler(Events::Event* event) {
	auto& c = static_cast<Events::AnyType<LineRender*>*>(event)->data;

	auto material = c->GetMaterial() ? c->GetMaterial() : defaultMaterial;
	auto shader = material->GetShader();

	if (batches.find(shader) == batches.end()) {
		InitializeShader(shader);
	}

	auto& list = batches[shader][material];

	if (c->IsActive()) {
		Helpers::Insert(list, c);
	} else {
		Helpers::Remove(list, c);
	}
}

void LineRenderer::MaterialHandler(Events::Event * event) {
	auto changeEvent = static_cast<Events::MaterialChange*>(event);
	auto c = static_cast<LineRender*>(changeEvent->component);

	auto material = c->GetMaterial() ? c->GetMaterial() : defaultMaterial;
	auto shader = material->GetShader();

	if (batches.find(shader) == batches.end()) {
		InitializeShader(shader);
	}

	auto& previous = batches[shader][changeEvent->previous];
	auto& current = batches[shader][material];

	if (Helpers::Remove(previous, c)) {
		current.push_back(c);
	}
}

void LineRenderer::ShaderHandler(Events::Event * event) {
	auto changeEvent = static_cast<Events::ShaderChange*>(event);
	auto& material = changeEvent->material;
	
	auto previous = changeEvent->previous;
	auto current = material->GetShader();

	if (batches.find(current) == batches.end()) {
		InitializeShader(current);
	}

	batches[current][material] = batches[previous][material];
	batches[previous].erase(material);
}

void LineRenderer::GenerateLine() {
	float lineVertices[] = {
		0.f, 0.f,
		1.f, 1.f
	};

	unsigned VBO;
	glGenVertexArrays(1, &lineVAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(lineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), &lineVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vec2f), (void*)0);
}

void LineRenderer::InitializeShader(Shader * const shader) {
	shader->Use();

	glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);

	const unsigned unit = 4 * sizeof(float);
	const unsigned stride = sizeof(Instance);

	unsigned i = INSTANCE_LAYOUT_LOCATION;
	unsigned offset = 0;

	// offset	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(i++, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
	offset += sizeof(vec3f);
	// length	
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(i++, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
	offset += sizeof(vec3f);
	// tint	
	glEnableVertexAttribArray(3);
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

