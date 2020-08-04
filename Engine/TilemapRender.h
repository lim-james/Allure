#ifndef TILEMAP_RENDER_H
#define TILEMAP_RENDER_H

#include "Component.h"

#include "TilemapResource.h"
#include "Material.h"

#include <Math/Vectors.h>
#include <Events/Event.h>

struct TilemapRender : Component {

	using base_type = TilemapRender;

	vec4f tint;
	TilemapLayout layout;

	TilemapRender();

	void Initialize() override;
	Component* Clone() const override;
	void SetActive(bool const& state) override;
	void SetDynamic(bool const& state) override;

	Material::Base* const GetMaterial() const;
	void SetMaterial(Material::Base* _material);

	TilemapPalette GetPalette() const;
	void SetPalette(TilemapPalette const& newPalette);

private:

	Material::Base* material;
	TilemapPalette palette;

};

namespace Events {
	struct PaletteChange : Event {
		const TilemapPalette previous;
		TilemapRender * const component;

		PaletteChange(TilemapPalette const& previous, TilemapRender * const component)
			: previous(previous)
			, component(component) {}
	};
}

#endif
