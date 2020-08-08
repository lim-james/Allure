#include "AVButton.h"

#include "Layout.h"
#include "SpriteRender.h"
#include "Button.h"
#include "AVButtonHandler.h"
// utils
#include "Layers.h"
#include "LoadTexture.h"

Transform * AVButton::Create() {
	const unsigned entity = entities->Create();
	entities->SetLayer(entity, UI);

	Transform* const parent = entities->GetComponent<Transform>(entity);

	Layout* const layout = entities->AddComponent<Layout>(entity);
	layout->SetActive(true);

	SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
	render->SetActive(true);

	Button* const button = entities->AddComponent<Button>(entity);
	button->SetActive(true);
	button->scale = 2.f;

	AVButtonHandler* const avHandler = entities->AddComponent<AVButtonHandler>(entity);
	avHandler->SetActive(true);

	button->handlers[MOUSE_OVER].Bind(&AVButtonHandler::OnMouseOver, avHandler);
	button->handlers[MOUSE_OUT].Bind(&AVButtonHandler::OnMouseOut, avHandler);
	button->handlers[MOUSE_DOWN].Bind(&AVButtonHandler::OnMouseDown, avHandler);
	button->handlers[MOUSE_UP].Bind(&AVButtonHandler::OnMouseUp, avHandler);

	{
		const unsigned entity = entities->Create();
		entities->SetLayer(entity, UI);

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetParent(parent);
		transform->SetScale(2.f);

		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);
		render->SetMaterial(material);

		avHandler->outline = render;
	}

	return parent;
}
