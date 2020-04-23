#include "LineRenderer.h"

#include "Transform.h"

#include <Helpers/VectorHelpers.h>
#include <Events/EventsManager.h>
#include <GL/glew.h>

unsigned LineRenderer::dynamicVAO = 0;
unsigned LineRenderer::dynamicBuffer = 0;

LineRenderer::Batch::StaticData::StaticData() : VAO(0), count(0) { }

LineRenderer::~LineRenderer() {
	delete defaultMaterial;
}

void LineRenderer::Initialize(EntityManager * const manager) {
	Renderer::Initialize(manager);

	if (dynamicVAO == 0)
		GenerateLine(dynamicVAO);

	InitializeInstanceBuffer(dynamicBuffer);

	defaultMaterial = new Material::LineDefault;

	Events::EventsManager::GetInstance()->Subscribe("LINE_RENDER_ACTIVE", &LineRenderer::ActiveHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("LINE_RENDER_DYNAMIC", &LineRenderer::DynamicHandler, this);
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

		for (auto& materialPair : shaderPair.second) {
			materialPair.first->Use();
			RenderStatic(data, materialPair.second);
			RenderDynamic(data, materialPair.second);
		}
	}

	glEnable(GL_DEPTH_TEST);
}

void LineRenderer::InitializeInstanceBuffer(unsigned & instanceBuffer) {
	if (instanceBuffer) return;
		
	glGenBuffers(1, &instanceBuffer);
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

void LineRenderer::RenderStatic(RendererData const & data, Batch & batch) {
	Batch::StaticData& staticData = batch.staticData[data.camera];
	
	if (updateStatic) {
		if (batch.staticList.empty()) return;

		std::vector<Instance> instances;
		instances.reserve(batch.staticList.size());

		for (auto& c : batch.staticList) {
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

		if (instances.empty()) return;

		if (staticData.VAO == 0)
			GenerateLine(staticData.VAO);

		unsigned VBO = 0;
		InitializeInstanceBuffer(VBO);
		staticData.count = instances.size();

		glBindVertexArray(staticData.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, staticData.count * sizeof(Instance), &instances[0], GL_STATIC_DRAW);
		glDrawArraysInstanced(GL_LINES, 0, 2,staticData.count);
	} else {
		glBindVertexArray(staticData.VAO);
		glDrawArraysInstanced(GL_LINES, 0, 2, staticData.count);
	}
}

void LineRenderer::RenderDynamic(RendererData const & data, Batch const & batch) {
	if (batch.dynamicList.empty()) return;

	std::vector<Instance> instances;
	instances.reserve(batch.dynamicList.size());

	for (auto& c : batch.dynamicList) {
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

	if (instances.empty()) return;

	const unsigned count = instances.size();
	glBindVertexArray(dynamicVAO);
	glBindBuffer(GL_ARRAY_BUFFER, dynamicBuffer);
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(Instance), &instances[0], GL_STATIC_DRAW);
	glDrawArraysInstanced(GL_LINES, 0, 2, count);
}

void LineRenderer::ActiveHandler(Events::Event* event) {
	auto& c = static_cast<Events::AnyType<LineRender*>*>(event)->data;

	auto material = c->GetMaterial() ? c->GetMaterial() : defaultMaterial;
	auto shader = material->GetShader();

	auto& batch = batches[shader][material];
	auto& list = c->IsDynamic() ? batch.dynamicList : batch.staticList;
	if (!c->IsDynamic()) updateStatic = true;

	if (c->IsActive()) {
		Helpers::Insert(list, c);
	} else {
		Helpers::Remove(list, c);
	}
}

void LineRenderer::DynamicHandler(Events::Event * event) {
	auto& c = static_cast<Events::AnyType<LineRender*>*>(event)->data;

	if (!c->IsActive()) return;

	auto material = c->GetMaterial() ? c->GetMaterial() : defaultMaterial;
	auto shader = material->GetShader();

	auto& batch = batches[shader][material];

	auto& previousList = c->IsDynamic() ? batch.staticList : batch.dynamicList;
	auto& currentList = c->IsDynamic() ? batch.dynamicList : batch.staticList;

	if (Helpers::Remove(previousList, c)) {
		Helpers::Insert(currentList, c);
		updateStatic = true;
	}
}

void LineRenderer::MaterialHandler(Events::Event * event) {
	auto changeEvent = static_cast<Events::MaterialChange*>(event);
	auto c = static_cast<LineRender*>(changeEvent->component);

	auto material = c->GetMaterial() ? c->GetMaterial() : defaultMaterial;
	auto shader = material->GetShader();

	auto& previous = batches[shader][changeEvent->previous];
	auto& current = batches[shader][material];

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

void LineRenderer::ShaderHandler(Events::Event * event) {
	auto changeEvent = static_cast<Events::ShaderChange*>(event);
	auto& material = changeEvent->material;
	
	auto previous = changeEvent->previous;
	auto current = material->GetShader();

	batches[current][material] = batches[previous][material];
	batches[previous].erase(material);
}

void LineRenderer::GenerateLine(unsigned& VAO) {
	float lineVertices[] = {
		0.f, 0.f,
		1.f, 1.f
	};

	unsigned VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), &lineVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vec2f), (void*)0);
}

