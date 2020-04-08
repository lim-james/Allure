#ifndef SPRITE_RENDER_H
#define SPRITE_RENDER_H

#include "Component.h"

#include <Math/Vectors.h>
#include <Events/Event.h>

struct SpriteRender;

namespace Events {
	struct TextureChange : Event {
		const unsigned previous;
		SpriteRender * const component;

		TextureChange(unsigned const& previous, SpriteRender * const component)
			: previous(previous)
			, component(component) {}
	};
}

struct SpriteRender : Component {

	// size of UV in tilemap
	vec4f uvRect;
	vec4f tint;

	SpriteRender();

	void Initialize() override;
	Component* Clone() const override;
	void SetActive(bool const& state) override;

	unsigned const& GetTexture() const;
	void SetTexture(unsigned const& _texture);
	void SetTexture(std::string const& _texture);

	void SetTilemapSize(int const& width, int const& height);
	void SetCellRect(int const& x, int const& y, int const& width, int const& height);

private:

	unsigned texture;

	vec2f tilemapUnit;
	vec4f cellRect;

};

#endif
