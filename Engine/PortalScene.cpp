#include "PortalScene.h"

// component
#include "Camera.h"
#include "Light.h"
#include "EditorCamera.h"
#include "SpriteRender.h"
#include "MeshRender.h"
#include "PortalCamera.h"
// materials
#include "MeshUnlitMaterial.h"
#include "MeshDefaultMaterial.h"
#include "PortalMaterial.h"
// Utils
#include "LoadTexture.h"
#include "LoadModel.h"
#include "Layers.h"


void PortalScene::Awake() {
	Scene::Awake();
}

void PortalScene::Create() {
	Scene::Create();

	Material::MeshUnlit* opaque = new Material::MeshUnlit;
	opaque->alphaClipping = 0.01f;

	Material::MeshDefault* rustediron = new Material::MeshDefault;
	rustediron->useAlbedoMap = true;
	rustediron->albedoMap = Load::Texture2D("Files/Textures/rustediron2_basecolor.png");
	rustediron->useNormalMap = true;
	rustediron->normalMap = Load::Texture2D("Files/Textures/rustediron2_normal.png");
	rustediron->useMetallicMap = true;
	rustediron->metallicMap = Load::Texture2D("Files/Textures/rustediron2_metallic.png");
	rustediron->useRoughnessMap = true;
	rustediron->roughnessMap = Load::Texture2D("Files/Textures/rustediron2_roughness.png");
	rustediron->ao = 0.01f;

	Material::MeshDefault* streakedmetal = new Material::MeshDefault;
	streakedmetal->useAlbedoMap = true;
	streakedmetal->albedoMap = Load::Texture2D("Files/Textures/streakedmetal-albedo.png");
	streakedmetal->useMetallicMap = true;
	streakedmetal->metallicMap = Load::Texture2D("Files/Textures/streakedmetal-metalness.png");
	streakedmetal->useRoughnessMap = true;
	streakedmetal->roughnessMap = Load::Texture2D("Files/Textures/streakedmetal-roughness.png");
	streakedmetal->ao = 0.01f;

	Material::MeshDefault* hardwood = new Material::MeshDefault;
	hardwood->useAlbedoMap = true;
	hardwood->albedoMap = Load::Texture2D("Files/Textures/hardwood-brown-planks-albedo.png");
	hardwood->useNormalMap = true;
	hardwood->normalMap = Load::Texture2D("Files/Textures/hardwood-brown-planks-normal-ogl.png");
	hardwood->useMetallicMap = true;
	hardwood->metallicMap = Load::Texture2D("Files/Textures/hardwood-brown-planks-metallic.png");
	hardwood->useRoughnessMap = true;
	hardwood->roughnessMap = Load::Texture2D("Files/Textures/hardwood-brown-planks-roughness.png");
	hardwood->useAOMap = true;
	hardwood->aoMap = Load::Texture2D("Files/Textures/hardwood-brown-planks-ao.png");

	Material::MeshDefault* red = new Material::MeshDefault;
	red->albedo = vec4f(1.f, 0.f, 0.f, 1.f);
	red->ao = 1.f;

	Material::MeshDefault* blue = new Material::MeshDefault;
	blue->albedo = vec4f(0.f, 0.f, 1.f, 1.f);
	blue->ao = 1.f;

	Material::MeshDefault* green = new Material::MeshDefault;
	green->albedo = vec4f(0.f, 1.f, 0.f, 1.f);
	green->ao = 30.f;

	Material::Portal* redView = new Material::Portal;
	redView->flags += SURFACE_TRANSPARENT;
	Material::Portal* blueView = new Material::Portal;
	blueView->flags += SURFACE_TRANSPARENT;

	Mesh* const cube = Load::OBJ("Files/Models/cube.obj")->meshes[0];
	Mesh* const sphere = Load::OBJ("Files/Models/sphere.obj")->meshes[0];

	Camera* const camera = entities->GetComponent<Camera>(mainCamera);
	camera->SetSize(10.f);
	camera->clearColor = 0.9f;

	{
		EditorCamera* const editorCamera = entities->AddComponent<EditorCamera>(mainCamera);
		editorCamera->SetActive(true);
	}

	// portal camera
	const unsigned portalCamera = entities->Create();
	PortalCamera* const portal = entities->AddComponent<PortalCamera>(portalCamera);
	portal->SetActive(true);
	portal->mainCamera = entities->GetComponent<Transform>(mainCamera);

	{
		MeshRender* const render = entities->AddComponent<MeshRender>(portalCamera);
		render->SetActive(true);
		render->SetMaterial(green);
		render->SetMesh(cube);

		Camera* const pCam = entities->AddComponent<Camera>(portalCamera);
		pCam->SetActive(true);
		pCam->SetSize(10.f);
		pCam->clearColor = 0.9f;
		pCam->SetDepth(-1.f);

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
		pCam->SetFramebuffer(fb);

		redView->texture = fb->GetTexture();
		blueView->texture = fb->GetTexture();
	}

	// directional light
	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->rotation.x = -90.0f;
		transform->UpdateAxes();

		Light* const light = entities->AddComponent<Light>(entity);
		light->SetActive(true);
		light->color = vec4f(253.f / 255.f, 251.f / 255.f, 211.f / 255.f, 1.0f);
		light->type = LIGHT_DIRECTIONAL;
	}

	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->translation = vec3f(10.f, -3.f, 0.f);
		transform->scale = vec3f(12.f, 1.f, 20.f);
		transform->SetDynamic(false);

		MeshRender* const render = entities->AddComponent<MeshRender>(entity);
		render->SetActive(true);
		render->SetMaterial(red);
		render->SetMesh(cube);
		render->SetDynamic(false);
	}

	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->translation = vec3f(10.f, 0.f, 0.f);
		transform->scale = vec3f(8.f, 5.f, 1.f);
		transform->SetDynamic(false);

		portal->portal1 = transform;

		MeshRender* const render = entities->AddComponent<MeshRender>(entity);
		render->SetActive(true);
		render->SetMaterial(redView);
		render->SetMesh(cube);
		render->SetDynamic(false);
	}

	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->translation = vec3f(-10.f, -3.f, 0.f);
		transform->scale = vec3f(12.f, 1.f, 20.f);
		transform->SetDynamic(false);

		MeshRender* const render = entities->AddComponent<MeshRender>(entity);
		render->SetActive(true);
		render->SetMaterial(blue);
		render->SetMesh(cube);
		render->SetDynamic(false);
	}

	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->translation = vec3f(-10.f, 0.f, 0.f);
		transform->scale = vec3f(8.f, 5.f, 1.f);
		transform->SetDynamic(false);

		portal->portal2 = transform;

		MeshRender* const render = entities->AddComponent<MeshRender>(entity);
		render->SetActive(true);
		render->SetMaterial(blueView);
		render->SetMesh(cube);
		render->SetDynamic(false);
	}

	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->translation = vec3f(-11.f, -1.f, -5.f);
		transform->SetDynamic(false);

		MeshRender* const render = entities->AddComponent<MeshRender>(entity);
		render->SetActive(true);
		render->SetMaterial(rustediron);
		render->SetMesh(cube);
		render->SetDynamic(false);
	}

	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->translation = vec3f(-9.f, -1.f, -5.f);
		transform->SetDynamic(false);

		MeshRender* const render = entities->AddComponent<MeshRender>(entity);
		render->SetActive(true);
		render->SetMaterial(hardwood);
		render->SetMesh(cube);
		render->SetDynamic(false);
	}

	// spot light
	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->translation = vec3f(-10.f, 0.f, -5.f);
		transform->scale = 0.2f;
		transform->rotation.x = 90.f;
		transform->UpdateAxes();
		transform->SetDynamic(false);

		Light* const light = entities->AddComponent<Light>(entity);
		light->SetActive(true);
		light->intensity = 100.0f;
		light->outerCutOff = 45.f;
		light->range = 100.0f;
		light->type = LIGHT_SPOT;

		MeshRender* const render = entities->AddComponent<MeshRender>(entity);
		render->SetActive(true);
		render->SetMaterial(opaque);
		render->SetMesh(sphere);
		render->SetDynamic(false);
	}
}

