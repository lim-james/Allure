#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "Renderer.h"
#include "SpriteDefaultMaterial.h"
#include "SpriteRender.h"

#include <Events/Event.h>

class SpriteRenderer : public Renderer {

	struct Instance {
		vec4f uvRect;
		vec4f tint;
		mat4f model;
	};

	typedef std::map<unsigned, std::vector<SpriteRender*>> SpriteBatches;
	typedef std::map<Shader*, std::map<Material::Base*, SpriteBatches>> Batches;

	static const unsigned INSTANCE_LAYOUT_LOCATION = 2;
	static unsigned quadVAO;
	static unsigned instanceBuffer;

	Material::SpriteDefault* defaultMaterial;
	Batches batches;

public:

	~SpriteRenderer() override;

	void Initialize(EntityManager* const manager) override;
	void Render(RendererData const& data) override;

private:

	void ActiveHandler(Events::Event* event);
	void SpriteChangeHandler(Events::Event* event);
	void MaterialHandler(Events::Event* event);
	void ShaderHandler(Events::Event* event);

	void GenerateQuad();

};

#endif
