#ifndef RENDER_H
#define RENDER_H

#include "Component.h"

#include <Math/Vectors.h>
#include <Events/Event.h>

struct Render;

namespace Events {
	struct TextureChange : Event {
		const unsigned previous;
		Render * const component;

		TextureChange(unsigned const& previous, Render * const component)
			: previous(previous)
			, component(component) {}
	};
}

struct Render : Component {

	// size of UV in tilemap
	vec4f uvRect;
	vec4f tint;

	Render();

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
