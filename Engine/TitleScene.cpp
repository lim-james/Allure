#include "TitleScene.h"

// components
#include "SpriteRender.h"
#include "LineRender.h"
#include "ButtonAnimation.h"
#include "FollowCursor.h"
#include "TitleTransition.h"
// Systems
#include "ButtonSystem.h"
#include "ParticleSystem.h"
#include "AnimationSystem.h"
// Utils
#include "Layers.h"
#include "LoadFNT.h"
#include "LoadTGA.h"
// materials
#include "InvertMaterial.h"

#include <Events/EventsManager.h>

void TitleScene::Awake() {
	Scene::Awake();

	systems->Subscribe<ButtonSystem>(1);
	systems->Subscribe<ParticleSystem>(2);
	systems->Subscribe<AnimationSystem>(3);
}

void TitleScene::Create() {
	Scene::Create();

	// fonts
	auto microsoft = Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga");
	auto courierNew = Load::FNT("Files/Fonts/CourierNew.fnt", "Files/Fonts/CourierNew.tga");

	Camera* camera = entities->GetComponent<Camera>(mainCamera);
	camera->SetSize(10.f);
	camera->cullingMask -= UI;
	camera->projection = ORTHOGRAPHIC;

	{
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

		auto fb = new Framebuffer(1, 1);
		fb->Initialize(vec2u(1600, 900), { tData }, { rbData });

		camera->SetFramebuffer(fb);
	}

	// UI Camera
	{
		const unsigned entity = entities->Create();

		auto transform = entities->GetComponent<Transform>(entity);
		transform->translation.z = 10.f;

		auto camera = entities->AddComponent<Camera>(entity);
		camera->SetActive(true);
		camera->SetSize(10.f);
		camera->SetDepth(1);
		camera->shouldClear = false;
		camera->clearColor = vec4f(0.f);
		camera->projection = ORTHOGRAPHIC;
		camera->cullingMask = UI;
	}

	// manager 
	const unsigned manager = entities->Create();
	auto transition = entities->AddComponent<TitleTransition>(manager);
	transition->SetActive(true);

	{
		auto animation = entities->AddComponent<Animation>(manager);
		animation->SetActive(true);
	}

	// background
	{
		const unsigned entity = entities->Create();

		auto transform = entities->GetComponent<Transform>(entity);
		transform->scale = vec3f(80.0f, 45.0f, 0.0f);

		auto render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);
		render->SetSprite(Load::TGA("Files/Textures/tile.tga"));
		render->SetCellRect(0, 0, 32, 18);
		render->tint.a = 0.1f;
		transition->grid = render;
	}

	// title
	{
		const unsigned entity = entities->Create();
		entities->SetLayer(entity, UI);

		auto transform = entities->GetComponent<Transform>(entity);
		transform->translation.y = -0.25f;
		transition->titleTransform = transform;

		auto text = entities->AddComponent<Text>(entity);
		text->SetActive(true);
		text->SetFont(microsoft);
		text->text = "Allure 2D"; 
		text->color = vec4f(1.f);
		text->characterSpacing = 1;
	}

	// title underline
	{
		const unsigned entity = entities->Create();
		entities->SetLayer(entity, UI);

		auto transform = entities->GetComponent<Transform>(entity);
		transform->translation.y = -1.5f;

		auto line = entities->AddComponent<LineRender>(entity);
		line->SetActive(true);
		line->length = vec3f(3.f, 0.f, 0.f);
		line->offset = vec3f(-1.5f, 0.f, 0.f);
		transition->underline = line;
	}

	// raw image frame
	{
		//const unsigned entity = entities->Create();

		//auto transform = entities->GetComponent<Transform>(entity);
		//transform->translation = vec3f(12.f, 0.f, 1.f);
		//transform->scale = vec3f(16.f, 9.f, 0.f);

		//auto render = entities->AddComponent<SpriteRender>(entity);
		//render->SetActive(true);
		//render->SetSprite(camera->GetFramebuffer()->GetTexture());
		//auto invert = new Material::Invert;
		//render->SetMaterial(invert);
	}

	// open button
	{
		const unsigned entity = entities->Create();
		entities->SetLayer(entity, UI);

		auto transform = entities->GetComponent<Transform>(entity);
		transform->translation.y = -7.25f;
		transform->scale = vec3f(7.f, 1.5f, 0.f);
		transition->buttonTransform = transform;

		auto render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);
		render->tint = vec4f(1.f, 1.f, 1.f, 0.5f);

		auto text = entities->AddComponent<Text>(entity);
		text->SetActive(true);
		text->SetFont(microsoft);
		text->text = "Open canvas"; 
		text->scale = 0.5f;
		text->color = vec4f(0.f, 0.f, 0.f, 1.f);

		auto animation = entities->AddComponent<Animation>(entity);
		animation->SetActive(true);

		auto button = entities->AddComponent<Button>(entity);
		button->SetActive(true);
		button->handlers[MOUSE_CLICK].Bind(&TitleTransition::Transition, transition);
		button->handlers[MOUSE_OVER].Bind([transform]() {
			transform->Parse(transform->ToString());
		});

		auto buttonAnim = entities->AddComponent<ButtonAnimation>(entity);
		buttonAnim->SetActive(true);
	}

	{
		const unsigned entity = entities->Create();

		const auto emitter = entities->AddComponent<ParticleEmitter>(entity);
		emitter->SetActive(true);
		transition->emitter = emitter;

		emitter->lifetime = 5.f;
		emitter->lifetimeRange = 0.5f;

		emitter->positionRange = vec3f(0.f);

		emitter->angleRange = 180.f;

		emitter->speed = 5.f;

		emitter->accelRad = -5.f;

		emitter->startSize = vec3f(0.15f);
		emitter->endSize = vec3f(0.f);

		emitter->startColor = vec4f(0.5f, 0.0f, 0.5f, 1.f);
		emitter->startColorRange = vec4f(0.5f, 0.0f, 0.f, 0.f);

		emitter->endColor = vec4f(1.f, 0.5f, 0.0f, 1.f);
		emitter->endColorRange = vec4f(0.f, 0.5f, 0.0f, 0.f);

		auto follow = entities->AddComponent<FollowCursor>(entity);
		follow->SetActive(true);
		follow->camera = entities->GetComponent<Camera>(mainCamera);
	}
}
