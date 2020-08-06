#include "TilemapRenderer.h"

#include "Transform.h"

#include <Math/Mat4Transform.hpp>
#include <Helpers/VectorHelpers.h>
#include <Events/EventsManager.h>
#include <GL/glew.h>

unsigned TilemapRenderer::dynamicVAO = 0;
unsigned TilemapRenderer::dynamicBuffer = 0;

TilemapRenderer::~TilemapRenderer() {
	delete depthShader;
	delete defaultMaterial;
}

void TilemapRenderer::Initialize(EntityManager * const manager) {
	Renderer::Initialize(manager);

	if (dynamicVAO == 0)
		GenerateQuad(dynamicVAO);

	InitializeInstanceBuffer(dynamicVAO, dynamicBuffer);

	depthShader = new Shader("Files/Shaders/depth2D.vert", "Files/Shaders/depth2D.frag");
	depthShader->Use();
	depthShader->SetInt("tex", 0);

	defaultMaterial = new Material::TilemapDefault;

	EventsManager::Get()->Subscribe("TILEMAP_RENDER_ACTIVE", &TilemapRenderer::ActiveHandler, this);
	EventsManager::Get()->Subscribe("TILEMAP_RENDER_DYNAMIC", &TilemapRenderer::DynamicHandler, this);
	EventsManager::Get()->Subscribe("PALETTE_CHANGE", &TilemapRenderer::PaletteChangeHandler, this);
	EventsManager::Get()->Subscribe("TILEMAP_MATERIAL", &TilemapRenderer::MaterialHandler, this);
	EventsManager::Get()->Subscribe("MATERIAL_SHADER", &TilemapRenderer::ShaderHandler, this);
}

void TilemapRenderer::RenderDepth(RendererData const& data) {
	RenderDepthBatches(data, opaqueBatches);
	RenderDepthBatches(data, transparentBatches);
}

void TilemapRenderer::RenderOpaque(RendererData const & data) {
	RenderBatches(data, opaqueBatches);
}

void TilemapRenderer::RenderTransparent(RendererData const & data) {
	RenderBatches(data, transparentBatches);
}

void TilemapRenderer::PostRender() {
	updateStatic = false;
}

void TilemapRenderer::GenerateQuad(unsigned& VAO) {
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

void TilemapRenderer::InitializeInstanceBuffer(unsigned const& VAO, unsigned& instanceBuffer) {
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

void TilemapRenderer::RenderDepthBatches(RendererData const & data, Batches & batches) {
	depthShader->Use();
	depthShader->SetMatrix4("projection", data.projection);
	depthShader->SetMatrix4("view", data.view);

	for (auto& shaderPair : batches) {
		for (auto& materialPair : shaderPair.second) {
			if (!materialPair.first->flags.Is(RENDER_DEPTH)) 
				continue;

			for (auto& batchPair : materialPair.second) {
				auto const& tex = batchPair.first;
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, tex);
				depthShader->SetInt("useTex", tex);

				RenderStatic(data, batchPair.first, batchPair.second);
				RenderDynamic(data, batchPair.first, batchPair.second);
			}
		}
	}
}

void TilemapRenderer::RenderBatches(RendererData const& data, Batches& batches) {
	const unsigned lightCount = data.lights2D ? data.lights2D->size() : 0;

	for (auto& shaderPair : batches) {
		Shader* const shader = shaderPair.first;

		shader->Use();
		shader->SetMatrix4("projection", data.projection);
		shader->SetMatrix4("view", data.view);

		shader->SetInt("lightCount", lightCount);

		for (unsigned i = 0; i < lightCount; ++i) {
			Light* const light = data.lights2D->at(i);
			const std::string id = std::to_string(i);
			const std::string tag = "lights[" + id + "].";

			shader->SetInt(tag + "type", light->type);
			shader->SetFloat(tag + "range", light->range);
			shader->SetFloat(tag + "innerCutOff", cos(light->innerCutOff * Math::toRad));
			shader->SetFloat(tag + "outerCutOff", cos(light->outerCutOff * Math::toRad));
			shader->SetVector3(tag + "color", light->color);
			shader->SetFloat(tag + "intensity", light->intensity);
			shader->SetFloat(tag + "strength", light->strength);

			shader->SetInt(tag + "castShadows", light->CastShadows());

			Transform* const transform = entities->GetComponent<Transform>(light->entity);
			const vec3f lightPos = transform->GetWorldTranslation();

			shader->SetVector3(tag + "direction", transform->GetLocalFront());
			shader->SetVector3(tag + "position", lightPos);
			shader->SetVector3("lightPositions[" + id + "]", lightPos);
		}

		Component* const component = (Component*)data.object;
		Camera* const camera = dynamic_cast<Camera*>(component);

		if (camera) {
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, camera->GetDepthBuffer()->GetTexture());
		}

		for (auto& materialPair : shaderPair.second) {

			materialPair.first->Use();

			for (auto& batchPair : materialPair.second) {
				auto const& tex = batchPair.first;
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, tex);
				shader->SetInt("useTex", tex);

				RenderStatic(data, tex, batchPair.second);
				RenderDynamic(data, tex, batchPair.second);
			}
		}
	}
}

void TilemapRenderer::RenderStatic(RendererData const & data, unsigned const& texture, Batch& batch) {
	if (updateStatic || batch.staticData.find(data.object) == batch.staticData.end()) {
		if (batch.staticList.empty()) return;

		std::vector<Instance> instances;
		instances.reserve(batch.staticList.size());

		for (TilemapRender* const c : batch.staticList) {
			if (entities->GetLayer(c->entity) != data.cullingMask) {
				continue;
			}

			Transform* const transform = entities->GetComponent<Transform>(c->entity);

			Instance instance;
			instance.tint = c->tint;
			instance.model = transform->GetWorldTransform();
			instances.push_back(instance);
		}

		if (instances.empty()) return;

		Batch::StaticData& staticData = batch.staticData.at(data.object);

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
		Batch::StaticData& staticData = batch.staticData.at(data.object);
		glBindVertexArray(staticData.VAO);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, staticData.count);
	}

}

void TilemapRenderer::RenderDynamic(RendererData const& data, unsigned const& texture, Batch const& batch) {
	if (batch.dynamicList.empty()) return;

	std::vector<Instance> instances;
	instances.reserve(batch.dynamicList.size());

	for (TilemapRender* const c : batch.dynamicList) {
		if (entities->GetLayer(c->entity) != data.cullingMask) {
			continue;
		}

		Transform* const transform = entities->GetComponent<Transform>(c->entity);
		const mat4f worldTransform = transform->GetWorldTransform();

		for (unsigned const& group : c->GetPalette().GetTextureIndexes(texture)) {
			const TilemapTexture tmTex = c->GetPalette().GetTexture(group);
			if (c->layout.grids.size() <= group) continue;

			auto& grid = c->layout.grids[group];

			for (auto it = grid.rbegin(); it != grid.rend(); ++it) {
				Tile const& tile = *it;

				mat4f translation;
				Math::SetToTranslation(translation, vec3f(tile.position));

				const vec2f unit = tmTex.GetUnit();
				vec2f offset = unit * tile.uvOffset;
				offset.y = 1.f - offset.y - unit.y;

				Instance instance;
				instance.uvRect = vec4f(offset, unit);
				instance.tint = c->tint;
				instance.model = worldTransform * translation;
				instances.push_back(instance);
			}
		}
	}

	if (instances.empty()) return;

	const unsigned count = instances.size();
	glBindVertexArray(dynamicVAO);
	glBindBuffer(GL_ARRAY_BUFFER, dynamicBuffer);
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(Instance), &instances[0], GL_STATIC_DRAW);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, count);
}

void TilemapRenderer::ActiveHandler(Events::Event * event) {
	TilemapRender* const c = static_cast<Events::AnyType<TilemapRender*>*>(event)->data;

	if (!c->IsDynamic()) updateStatic = true;

	Material::Base* const material = c->GetMaterial() ? c->GetMaterial() : defaultMaterial;
	Shader* const shader = material->GetShader();

	Batches& batches = material->GetFlags() == SURFACE_TRANSPARENT ? transparentBatches : opaqueBatches;
	TilemapBatches& tmBatches = batches[shader][material];

	for (TilemapTexture& palette : c->GetPalette().GetTextures()) {
		Batch& batch = tmBatches[palette.texture];
		auto& list = c->IsDynamic() ? batch.dynamicList : batch.staticList;

		if (c->IsActive())
			Helpers::Insert(list, c);
		else
			Helpers::Remove(list, c);
	}
}

void TilemapRenderer::DynamicHandler(Events::Event * event) {
	TilemapRender* const c = static_cast<Events::AnyType<TilemapRender*>*>(event)->data;

	if (!c->IsActive()) return;

	Material::Base* const material = c->GetMaterial() ? c->GetMaterial() : defaultMaterial;
	Shader* const shader = material->GetShader();

	Batches& batches = material->GetFlags() == SURFACE_TRANSPARENT ? transparentBatches : opaqueBatches;
	TilemapBatches& tmBatches = batches[shader][material];

	for (TilemapTexture& palette : c->GetPalette().GetTextures()) {
		Batch& batch = tmBatches[palette.texture];
		auto& previousList = c->IsDynamic() ? batch.staticList : batch.dynamicList;
		auto& currentList = c->IsDynamic() ? batch.dynamicList : batch.staticList;

		if (Helpers::Remove(previousList, c)) {
			Helpers::Insert(currentList, c);
			updateStatic = true;
		}
	}
}

void TilemapRenderer::PaletteChangeHandler(Events::Event* event) {
	const auto changeEvent = static_cast<Events::PaletteChange*>(event);
	TilemapRender* const c = changeEvent->component;

	if (!c->IsActive()) return;

	Material::Base* const material = c->GetMaterial() ? c->GetMaterial() : defaultMaterial;
	Shader* const shader = material->GetShader();

	Batches& batches = material->GetFlags() == SURFACE_TRANSPARENT ? transparentBatches : opaqueBatches;
	TilemapBatches& batch = batches[shader][material];

	//Batch& previous = batch[changeEvent->previous];
	//Batch& current = batch[c->GetTilemap()];

	//if (c->IsDynamic()) {
	//	if (Helpers::Remove(previous.dynamicList, c)) {
	//		current.dynamicList.push_back(c);
	//	}
	//} else {
	//	if (Helpers::Remove(previous.staticList, c)) {
	//		current.staticList.push_back(c);
	//		updateStatic = true;
	//	}
	//}
}

void TilemapRenderer::MaterialHandler(Events::Event * event) {
	const auto changeEvent = static_cast<Events::MaterialChange*>(event);
	auto c = static_cast<TilemapRender*>(changeEvent->component);

	if (!c->IsActive()) return;

	Material::Base* const material = c->GetMaterial() ? c->GetMaterial() : defaultMaterial;
	Shader* const shader = material->GetShader();

	Material::Base* const previousMaterial = changeEvent->previous ? changeEvent->previous : defaultMaterial;
	Batches& previousBatches = previousMaterial->GetFlags() == SURFACE_TRANSPARENT ? transparentBatches : opaqueBatches;
	Batches& currentBatches = material->GetFlags() == SURFACE_TRANSPARENT ? transparentBatches : opaqueBatches;

	TilemapBatches& tmPrevious = previousBatches[previousMaterial->GetShader()][previousMaterial];
	TilemapBatches& tmCurrent = currentBatches[shader][material];
	
	for (TilemapTexture& palette : c->GetPalette().GetTextures()) {
		Batch& previous = tmPrevious[palette.texture];
		Batch& current = tmCurrent[palette.texture];

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

}

void TilemapRenderer::ShaderHandler(Events::Event * event) {
	const auto changeEvent = static_cast<Events::ShaderChange*>(event);
	Material::Base* const material = changeEvent->material;

	Shader* const previous = changeEvent->previous;
	Shader* const current = material->GetShader();

	Batches& batches = material->GetFlags() == SURFACE_TRANSPARENT ? transparentBatches : opaqueBatches;
	batches[current][material] = batches[previous][material];
	batches[previous].erase(material);
	updateStatic = true;
}
