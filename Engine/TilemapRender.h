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
	TilemapPalette palette;
	TilemapLayout layout;

	TilemapRender();

	void Initialize() override;
	Component* Clone() const override;
	void SetActive(bool const& state) override;
	void SetDynamic(bool const& state) override;

	Material::Base* const GetMaterial() const;
	void SetMaterial(Material::Base* _material);

private:

	Material::Base* material;

};

//namespace Events {
//	struct SpriteChange : Event {
//		const unsigned previous;
//		SpriteRender * const component;
//
//		SpriteChange(unsigned const& previous, SpriteRender * const component)
//			: previous(previous)
//			, component(component) {}
//	};
//}

#endif
