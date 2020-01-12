#include "RenderSystem.h"

#include "EntityManager.h"
#include "Transform.h"

#include "InputEvents.h"

#include <Events/EventsManager.h>
#include <Helpers/VectorHelpers.h>
#include <Math/MatrixTransform.hpp>
#include <MACROS.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

unsigned RenderSystem::instanceBuffer = 0;
unsigned RenderSystem::quadVAO = 0;
unsigned RenderSystem::lineVAO = 0;

bool operator==(const Instance& lhs, const Instance& rhs) {
	return lhs.component == rhs.component;
}

RenderSystem::~RenderSystem() {
	delete mainShader;
	delete lineShader;
	delete textShader;

	delete curveShader;
	delete mainFBO;
}

void RenderSystem::Initialize() {
	debugging = false;

	if (instanceBuffer == 0)
		glGenBuffers(1, &instanceBuffer);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (quadVAO == 0)
		GenerateQuad();

	if (lineVAO == 0)
		GenerateLine();

	mainShader = new Shader("Files/Shaders/standard2D.vert", "Files/Shaders/standard2D.frag");
	mainShader->Use();
	mainShader->SetInt("tex", 0);

	lineShader = new Shader("Files/Shaders/line.vert", "Files/Shaders/standard2D.frag");
	lineShader->Use();
	lineShader->SetInt("tex", 0);

	textShader = new Shader("Files/Shaders/text.vert", "Files/Shaders/standard2D.frag");
	textShader->Use();
	textShader->SetInt("tex", 0);

	curveShader = new Shader("Files/Shaders/fb.vert", "Files/Shaders/curve.frag");
	curveShader->Use();
	curveShader->SetInt("tex", 0);

	TextureData tData;
	tData.level = 0;
	tData.internalFormat = GL_RGB16F;
	tData.border = 0;
	tData.format = GL_RGBA;
	tData.type = GL_UNSIGNED_BYTE;
	tData.attachment = GL_COLOR_ATTACHMENT0;
	tData.parameters.push_back({ GL_TEXTURE_MIN_FILTER, GL_LINEAR });
	tData.parameters.push_back({ GL_TEXTURE_MAG_FILTER, GL_LINEAR });
	tData.parameters.push_back({ GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE });
	tData.parameters.push_back({ GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE });

	RenderBufferData rbData;
	rbData.internalFormat = GL_DEPTH24_STENCIL8;
	rbData.attachmentFormat = GL_DEPTH_STENCIL_ATTACHMENT;

	mainFBO = new Framebuffer(1, 1);
	mainFBO->Initialize(vec2u(1600, 900), { tData }, { rbData });

	lines.push_back({ vec2f(0.f), vec2f(1.f), vec4f(0.f, 1.f, 0.f, 1.f) });

	Events::EventsManager::GetInstance()->Subscribe("KEY_INPUT", &RenderSystem::KeyHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("CAMERA_ACTIVE", &RenderSystem::CameraActiveHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("CAMERA_DEPTH", &RenderSystem::CameraDepthHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("RENDER_ACTIVE", &RenderSystem::RenderActiveHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("TEXTURE_CHANGE", &RenderSystem::TextureChangeHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("TEXT_ACTIVE", &RenderSystem::TextActiveHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("TEXT_FONT", &RenderSystem::TextFontHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("WINDOW_RESIZE", &RenderSystem::ResizeHandler, this);
}

void RenderSystem::Update(const float& dt) {
	glViewport(0, 0, static_cast<GLsizei>(windowSize.w), static_cast<GLsizei>(windowSize.h));
	glScissor(0, 0, static_cast<GLsizei>(windowSize.w), static_cast<GLsizei>(windowSize.h));
	glClearColor(0, 0, 0, 0);

	for (auto& cam : cameras) {
		glBindVertexArray(quadVAO);

		mainShader->Use();
		const auto& viewport = cam->GetViewport();
		const auto& projection = cam->GetProjectionMatrix();
		const auto& lookAt = entities->GetComponent<Transform>(cam->entity)->GetLocalLookAt();

		const Math::vec2<GLint> origin(
			static_cast<GLint>(viewport.origin.x),
			static_cast<GLint>(viewport.origin.y)
		);

		const Math::vec2<GLint> size(
			static_cast<GLsizei>(viewport.size.w),
			static_cast<GLsizei>(viewport.size.h)
		);

		glViewport(origin.x, origin.y, size.x, size.y);
		glScissor(origin.x, origin.y, size.x, size.y);
		glClearColor(cam->clearColor.r, cam->clearColor.g, cam->clearColor.b, cam->clearColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mainShader->SetMatrix4("projection", projection);
		mainShader->SetMatrix4("view", lookAt);

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

				//for (auto& quad : transform->quads) {
				//	mat4f model;
				//	Math::SetToTranslation(model, vec3f(quad->position, 0.f));
				//	Math::Scale(model, vec3f(quad->size * 2.f - 0.01f, 1.f));
				//	batch.push_back({
				//		nullptr,
				//		vec4f(),
				//		vec4f(vec3f(0.f), 1.f),
				//		model	
				//	});
				//}
			}

			count = batch.size();
			const unsigned unit = 4 * sizeof(float);
			const unsigned stride = sizeof(Instance);

			glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
			glBufferData(GL_ARRAY_BUFFER, count * stride, &batch[0], GL_STATIC_DRAW);

			unsigned i, offset;
			i = INSTANCE_LAYOUT_LOCATION;
			offset = sizeof(Render*);

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
			mainShader->SetInt("useTex", tex);
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, count);
		}

		glDisable(GL_DEPTH_TEST);

		const unsigned count = lines.size();
		if (count) {
			glLineWidth(cam->size * .25f);

			glBindVertexArray(lineVAO);
			lineShader->Use();
			lineShader->SetMatrix4("projection", projection);
			lineShader->SetMatrix4("view", lookAt);

			const unsigned stride = sizeof(Line);

			glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
			glBufferData(GL_ARRAY_BUFFER, count * stride, &lines[0], GL_STATIC_DRAW);

			// offset	
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, 0);
			glVertexAttribDivisor(1, 1);
			// length	
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(vec2f));
			glVertexAttribDivisor(2, 1);
			// tint	
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(vec4f));
			glVertexAttribDivisor(3, 1);

			lineShader->SetInt("useTex", 0);
			glDrawArraysInstanced(GL_LINES, 0, 2, count);

			lines.clear();
		}

		textShader->Use();
		textShader->SetMatrix4("projection", projection);
		textShader->SetMatrix4("view", lookAt);

		for (auto& textPair : textBatches) {
			auto& font = textPair.first;
			if (!font) continue;

			const auto tex = font->texture;
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, tex);
			textShader->SetInt("useTex", tex);

			glBindVertexArray(font->VAO);

			for (auto& text : textPair.second) {
				textShader->SetVector4("color", text->color);
				auto transform = entities->GetComponent<Transform>(text->entity);

				const float scale = text->scale;

				std::vector<float> lineOffset;
				vec2f size(0.f);

				const auto& content = text->text;

				for (unsigned i = 0; i <= content.size(); ++i) {
					auto& c = content[i];

					switch (c) {
					case '\n':
					case '\0':
						++size.y;
						switch (text->paragraphAlignment) {
						case PARAGRAPH_CENTER:
							lineOffset.push_back(size.x * scale * 0.5f);
							break;
						case PARAGRAPH_RIGHT:
							lineOffset.push_back(transform->scale.x * -0.5f - size.x * -scale);
							break;
						default:
							lineOffset.push_back(transform->scale.x * 0.5f);
							break;
						}
						size.x = 0.f;
						break;
					default:
						size.x += font->characters[c].xAdvance;
						break;
					}
				}
				size.y *= font->lineHeight * text->lineSpacing * text->scale;

				const vec3f translation = transform->GetWorldTranslation();
				vec3f position(0.f);
				position.x = translation.x - lineOffset[0];

				switch (text->verticalAlignment) {
				case ALIGN_MIDDLE:
					position.y = translation.y + size.y * 0.5f;
					break;
				case ALIGN_BOTTOM:
					position.y = translation.y - transform->scale.y * 0.5f + size.y;
					break;
				default:
					position.y = translation.y + transform->scale.y * 0.5f;
					break;
				}

				int lineNumer = 0;
				for (auto& c : content) {
					if (c == '\0') continue;

					switch (c) {
					case '\n':
						position.y -= font->lineHeight * text->lineSpacing * text->scale;
						position.x = translation.x - lineOffset[++lineNumer];
						break;
					default:
						const auto& character = font->characters[c];
						const vec3f offset = character.rect.origin * scale;

						mat4f model;
						Math::SetToTranslation(model, position + offset);
						if (text->scale != 1.f)
							Math::Scale(model, vec3f(scale));
						textShader->SetMatrix4("model", model);

						const int index = character.index * 6;
						glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(index * sizeof(unsigned)));

						position.x += character.xAdvance * text->characterSpacing * scale;
						break;
					}
				}
			}
		}

		glEnable(GL_DEPTH_TEST);
	}
}

void RenderSystem::KeyHandler(Events::Event * event) {
	Events::KeyInput* input = static_cast<Events::KeyInput*>(event);

	if (input->key == GLFW_KEY_ENTER && input->action == GLFW_PRESS) {
		debugging = !debugging;

		if (debugging)
			Events::EventsManager::GetInstance()->Subscribe("DRAW_LINE", &RenderSystem::DrawLineHandler, this);
		else
			Events::EventsManager::GetInstance()->Unsubscribe("DRAW_LINE", this);
	}
}

void RenderSystem::CameraActiveHandler(Events::Event* event) {
	auto& c = static_cast<Events::AnyType<Camera*>*>(event)->data;

	if (c->IsActive()) {
		for (unsigned i = 0; i < cameras.size(); ++i) {
			if (cameras[i] == c) {
				return;
			} else if (cameras[i]->depth >= c->depth) {
				cameras.insert(cameras.begin() + i, c);
				return;
			}
		}
		cameras.push_back(c);
	} else {
		Helpers::Remove(cameras, c);
	}
}

void RenderSystem::CameraDepthHandler(Events::Event* event) {
	auto& c = static_cast<Events::AnyType<Camera*>*>(event)->data;

	if (c->IsActive()) {
		cameras.erase(vfind(cameras, c));
		for (unsigned i = 0; i < cameras.size(); ++i) {
			if (cameras[i]->depth >= c->depth) {
				cameras.insert(cameras.begin() + i, c);
				return;
			}
		}
		cameras.push_back(c);
	}
}

void RenderSystem::RenderActiveHandler(Events::Event* event) {
	auto& c = static_cast<Events::AnyType<Render*>*>(event)->data;
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

void RenderSystem::TextureChangeHandler(Events::Event* event) {
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

void RenderSystem::DrawLineHandler(Events::Event* event) {
	lines.push_back(
		static_cast<Events::AnyType<Line>*>(event)->data
	);
}

void RenderSystem::TextActiveHandler(Events::Event* event) {
	auto& c = static_cast<Events::AnyType<Text*>*>(event)->data;
	const auto font = c->GetFont();
	if (!font) return;

	auto& textList = textBatches[font];

	if (c->IsActive()) {
		Helpers::Insert(textList, c);
	} else {
		Helpers::Remove(textList, c);
	}
}

void RenderSystem::TextFontHandler(Events::Event* event) {
	const auto change = static_cast<Events::FontChange*>(event);
	auto& c = change->component;
	auto previous = change->previous;
	auto current = c->GetFont();

	if (!previous) {
		textBatches[current].push_back(c);
	} else if (!current) {
		auto& textList = textBatches[previous];
		textList.erase(vfind(textList, c));
	} else {
		auto& prevList = textBatches[previous];
		prevList.erase(vfind(prevList, c));
		textBatches[current].push_back(c);
	}
}

void RenderSystem::ResizeHandler(Events::Event* event) {
	windowSize = static_cast<Events::AnyType<vec2i>*>(event)->data;
	mainFBO->Resize(windowSize);
}

void RenderSystem::GenerateQuad() {
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

void RenderSystem::GenerateLine() {
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
