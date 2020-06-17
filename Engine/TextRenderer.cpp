#include "TextRenderer.h"

#include "Transform.h"

#include <Math/Mat4Transform.hpp>
#include <Helpers/VectorHelpers.h>
#include <Events/EventsManager.h>
#include <GL/glew.h>

TextRenderer::~TextRenderer() {
	delete shader;
}

void TextRenderer::Initialize(EntityManager * const manager) {
	Renderer::Initialize(manager);

	shader = new Shader("Files/Shaders/text.vert", "Files/Shaders/standard2D.frag");
	shader->Use();
	shader->SetInt("tex", 0);

	EventsManager::Get()->Subscribe("TEXT_ACTIVE", &TextRenderer::ActiveHandler, this);
	EventsManager::Get()->Subscribe("TEXT_FONT", &TextRenderer::FontHandler, this);
}

void TextRenderer::RenderDepth(RendererData const & data) {}

void TextRenderer::RenderOpaque(RendererData const & data) {}

void TextRenderer::RenderTransparent(RendererData const& data) {
	glDisable(GL_DEPTH_TEST);

	shader->Use();
	shader->SetMatrix4("projection", data.projection);
	shader->SetMatrix4("view", data.view);

	for (auto& textPair : textBatches) {
		auto& font = textPair.first;
		if (!font) continue;

		const auto tex = font->texture;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		shader->SetInt("useTex", tex);

		glBindVertexArray(font->VAO);

		for (auto& text : textPair.second) {
			if (entities->GetLayer(text->entity) != data.cullingMask) {
				continue;
			}

			shader->SetVector4("color", text->color);
			auto transform = entities->GetComponent<Transform>(text->entity);

			const float scale = text->scale;

			std::vector<float> lineOffset;
			vec2f size(0.f);

			auto const& content = text->text;

			for (unsigned i = 0; i <= content.size(); ++i) {
				const char c = content[i];

				const float width = size.x * text->characterSpacing * scale;
				switch (c) {
				case '\n':
				case '\0':
					++size.y;

					switch (text->paragraphAlignment) {
					case PARAGRAPH_CENTER:
						lineOffset.push_back(width * 0.5f);
						break;
					case PARAGRAPH_RIGHT:
						lineOffset.push_back(transform->scale.x * -0.5f + width);
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

			const vec3f translation = transform->GetWorldTranslation() + text->offset;
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
					auto const& character = font->characters[c];
					const vec3f offset = character.rect.origin * scale;

					mat4f model;
					Math::SetToTranslation(model, position + offset);
					if (text->scale != 1.f)
						Math::Scale(model, vec3f(scale));
					shader->SetMatrix4("model", model);

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

void TextRenderer::ActiveHandler(Events::Event* event) {
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

void TextRenderer::FontHandler(Events::Event* event) {
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
