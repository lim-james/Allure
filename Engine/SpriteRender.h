#ifndef SPRITE_RENDER_H
#define SPRITE_RENDER_H

#include "Component.h"

#include "Material.h"

#include <Math/Vectors.h>
#include <Events/Event.h>

struct SpriteRender : Component {

	// size of UV in tilemap
	vec4f uvRect;
	vec4f tint;

	SpriteRender();

	void Initialize() override;
	Component* Clone() const override;
	void SetActive(bool const& state) override;

	unsigned const& GetSprite() const;
	void SetSprite(unsigned const& _sprite);

	void SetTilemapSize(int const& width, int const& height);
	void SetCellRect(int const& x, int const& y, int const& width, int const& height);

	Material::Base* const GetMaterial() const;
	void SetMaterial(Material::Base* _material);

private:

	unsigned sprite;

	vec2f tilemapUnit;
	vec4f cellRect;

	Material::Base* material;

};

namespace Events {
	struct SpriteChange : Event {
		const unsigned previous;
		SpriteRender * const component;

		SpriteChange(unsigned const& previous, SpriteRender * const component)
			: previous(previous)
			, component(component) {}
	};
}

#endif
