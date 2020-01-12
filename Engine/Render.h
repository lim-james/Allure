#ifndef RENDER_H
#define RENDER_H

#include "Component.h"

#include <Math/Vectors.hpp>
#include <Events/Event.h>

struct Render;

namespace Events {
	struct TextureChange : Event {
		const unsigned previous;
		Render * const component;

		TextureChange(const unsigned& previous, Render * const component)
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
	void SetActive(const bool& state) override;

	const unsigned& GetTexture() const;
	void SetTexture(const unsigned& _texture);
	void SetTexture(const std::string& _texture);

	void SetTilemapSize(const int& width, const int& height);
	void SetCellRect(const int& x, const int& y, const int& width, const int& height);

private:

	unsigned texture;

	vec2f tilemapUnit;
	vec4f cellRect;

};

#endif
