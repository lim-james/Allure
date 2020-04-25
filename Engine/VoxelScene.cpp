#include "VoxelScene.h"

// component
#include "Camera.h"
#include "Light.h"
#include "EditorCamera.h"
#include "VoxelRenderer.h"
#include "SpriteRenderer.h"
// UI components
#include "Panel.h"
// Utils
#include "Layers.h"

#include <Math/Random.hpp>

void VoxelScene::Create() {
	Scene::Create();

	Camera* const camera = entities->GetComponent<Camera>(mainCamera);
	camera->SetSize(12.f);
	camera->clearColor = 1.f;
	camera->SetMatch(1.f);
	camera->projection = ORTHOGRAPHIC;
	camera->cullingMask = UI;

	// editor view
	const unsigned editorView = CreateEditor();
	{
		const unsigned entity = entities->Create();
		entities->SetLayer(entity, UI);

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->scale = vec3f(16.f, 9.f, 0.f);

		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);
		render->SetSprite(editorView);

		Panel* const panel = entities->AddComponent<Panel>(entity);
		panel->SetActive(true);
		panel->camera = camera;
	}

	{
		const unsigned entity = entities->Create();
		entities->SetLayer(entity, UI);

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->translation.x = -10.f;
		transform->scale = vec3f(4.f, 9.f, 0.f);

		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);
		render->tint = vec4f(1.f, 0.f, 0.f, 1.f);

		Panel* const panel = entities->AddComponent<Panel>(entity);
		panel->SetActive(true);
		panel->camera = camera;
	}
}

unsigned VoxelScene::CreateEditor() {
	unsigned editorView = 0;
	// editor camera
	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->translation.z = 10.f;

		Camera* const camera = entities->AddComponent<Camera>(entity);
		camera->SetActive(true);
		camera->SetSize(10.f);
		camera->clearColor = 0.3f;
		camera->cullingMask -= UI;
		camera->isHidden = true;

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

		Framebuffer* const fb = new Framebuffer(1, 1);
		fb->Initialize(vec2u(1600, 900), { tData }, { rbData });
		camera->SetFramebuffer(fb);
		editorView = fb->GetTexture();

		EditorCamera* const editorControls = entities->AddComponent<EditorCamera>(entity);
		editorControls->SetActive(true);
	}

	// directional light
	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->rotation = vec3f(50.f, -30.f, 0.f);
		transform->UpdateAxes();

		Light* const light = entities->AddComponent<Light>(entity);
		light->SetActive(true);
		light->color = vec4f(253.f / 255.f, 251.f / 255.f, 211.f / 255.f, 1.0f);
		light->type = LIGHT_DIRECTIONAL;
	}

	const float size = 10.f;

	for (float x = -size; x <= size; ++x) { 
		for (float z = -size; z <= size; ++z) {
			const unsigned entity = entities->Create();

			Transform* const transform = entities->GetComponent<Transform>(entity);
			transform->translation = vec3f(x, -1.f, z);
			transform->SetDynamic(false);

			VoxelRender* const render = entities->AddComponent<VoxelRender>(entity);
			render->SetActive(true);
			render->tint = vec4f(Math::RandValue(), Math::RandValue(), Math::RandValue(), 1.f);
			render->SetDynamic(false);
		}
	}

	return editorView;
}
