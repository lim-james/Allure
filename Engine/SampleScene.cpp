#include "SampleScene.h"

// component
#include "Camera.h"
#include "Light.h"
#include "Text.h"
#include "EditorCamera.h"
#include "SpriteRender.h"
#include "MeshRender.h"
#include "Orbit.h"
#include "TestAudio.h"
#include "FPSCounter.h"
// post vfx
#include "Bloom.h"
#include "CurveDisplay.h"
#include "Pixelated.h"
// systems
#include "AudioSystem.h"
#include "LayoutSystem.h"
#include "ColliderSystem.h"
// Utils
#include "LoadTexture.h"
#include "LoadModel.h"
#include "LoadFNT.h"
#include "Layers.h"

void SampleScene::Awake() {
	Scene::Awake();

	systems->Subscribe<AudioSystem>(1);
	systems->Subscribe<LayoutSystem>(2);
	systems->Subscribe<ColliderSystem>(3);
}

void SampleScene::Create() {
	Scene::Create();

	Font* const courierNew = Load::FNT("Files/Fonts/CourierNew.fnt", "Files/Fonts/CourierNew.tga");

	opaque = new Material::MeshUnlit; 
	opaque->alphaClipping = 0.01f;

	rustediron = new Material::MeshDefault;
	//resources->Track(rustediron);
	rustediron->useAlbedoMap = true;
	rustediron->albedoMap = Load::Texture2D("Files/Textures/rustediron2_basecolor.png");
	rustediron->useNormalMap = true;
	rustediron->normalMap = Load::Texture2D("Files/Textures/rustediron2_normal.png");
	rustediron->useMetallicMap = true;
	rustediron->metallicMap = Load::Texture2D("Files/Textures/rustediron2_metallic.png");
	rustediron->useRoughnessMap = true;
	rustediron->roughnessMap = Load::Texture2D("Files/Textures/rustediron2_roughness.png");
	rustediron->ao = 0.01f;

	streakedmetal = new Material::MeshDefault;
	//resources->Track(streakedmetal);
	streakedmetal->useAlbedoMap = true;
	streakedmetal->albedoMap = Load::Texture2D("Files/Textures/streakedmetal-albedo.png");
	streakedmetal->useMetallicMap = true;
	streakedmetal->metallicMap = Load::Texture2D("Files/Textures/streakedmetal-metalness.png");
	streakedmetal->useRoughnessMap = true;
	streakedmetal->roughnessMap = Load::Texture2D("Files/Textures/streakedmetal-roughness.png");
	streakedmetal->ao = 0.01f;

	hardwood = new Material::MeshDefault;
	//resources->Track(hardwood);
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

	white = new Material::MeshDefault;
	//resources->Track(white);
	white->albedo = vec4f(1.f);
	white->ao = 0.1f;

	const unsigned skyboxMap = Load::Cubemap({
		"Files/Textures/skybox/right.jpg",
		"Files/Textures/skybox/left.jpg",
		"Files/Textures/skybox/top.jpg",
		"Files/Textures/skybox/bottom.jpg",
		"Files/Textures/skybox/front.jpg",
		"Files/Textures/skybox/back.jpg"
	});

	window = new Material::MeshDefault;
	//resources->Track(window);
	window->albedo = vec4f(1.f, 1.f, 1.f, 0.8f);
	window->useMetallicMap = true;
	window->metallicMap = Load::Texture2D("Files/Textures/streakedmetal-metalness.png");
	window->useRoughnessMap = true;
	window->roughnessMap = Load::Texture2D("Files/Textures/streakedmetal-roughness.png");
	//window->refractiveIndex = 1.0f;
	//window->environment = skyboxMap;

	skybox = new Material::Skybox;
	//resources->Track(skybox);
	skybox->flags += FRONT_FACE;
	skybox->cubemap = skyboxMap;

	Mesh* const cube = Load::OBJ("Files/Models/cube.obj")->meshes[0];
	Mesh* const sphere = Load::OBJ("Files/Models/sphere.obj")->meshes[0];

	Camera* camera = entities->GetComponent<Camera>(mainCamera);
	camera->SetSize(10.f);
	camera->clearColor = vec4f(0.f, 0.f, 0.f, 1.f);
	camera->cullingMask -= UI;

	{
		EditorCamera* const editorCamera = entities->AddComponent<EditorCamera>(mainCamera);
		editorCamera->SetActive(true);

		AudioListener* const listener = entities->AddComponent<AudioListener>(mainCamera);
		listener->SetActive(true);
	}

	// UI Camera
	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetLocalTranslation(vec3f(0.f, 0.f, 10.f));

		Camera* const uiCamera = entities->AddComponent<Camera>(entity);
		uiCamera->SetActive(true);
		uiCamera->SetSize(10.f);
		uiCamera->SetDepth(1);
		uiCamera->shouldClear = false;
		uiCamera->projection = ORTHOGRAPHIC;
		uiCamera->cullingMask = UI;
	}

	// directional light
	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetLocalRotation(vec3f(50.f, -30.f, 0.f));
		
		Light* const light = entities->AddComponent<Light>(entity);
		light->SetActive(true);
		light->color = vec4f(253.f / 255.f, 251.f / 255.f, 211.f / 255.f, 1.0f);
		light->type = LIGHT_DIRECTIONAL;
	}

	// skybox
	{
		//const unsigned entity = entities->Create();

		//Transform* const transform = entities->GetComponent<Transform>(entity);
		//transform->scale = 50.f;
		//transform->SetDynamic(false);

		//MeshRender* const render = entities->AddComponent<MeshRender>(entity);
		//render->SetActive(true);
		//render->SetMaterial(skybox);
		//render->SetMesh(cube);
		//render->SetDynamic(false);
	}

	// FPS counter
	{
		const unsigned entity = entities->Create();
		entities->SetLayer(entity, UI);

		Layout* const layout = entities->AddComponent<Layout>(entity);
		layout->SetActive(true);
		layout->AddConstraint(Constraint(LEFT_ANCHOR, nullptr, LEFT_ANCHOR, 1.0, 1.0, camera));
		layout->AddConstraint(Constraint(TOP_ANCHOR, nullptr, TOP_ANCHOR, 1.0, -1.0, camera));

		Text* const text = entities->AddComponent<Text>(entity);
		text->SetActive(true);
		text->SetFont(courierNew);
		text->text = "60";

		FPSCounter* const fps = entities->AddComponent<FPSCounter>(entity);
		fps->SetActive(true);
	}

	// post processing volume
	{
		const unsigned volume = entities->Create();
		entities->AddComponent<Bloom>(volume)->SetActive(true);
		//entities->AddComponent<Pixelated>(volume)->SetActive(true);
		//entities->AddComponent<CurveDisplay>(volume)->SetActive(true);
	}

	// spot light
	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetLocalTranslation(vec2f(2.f, 3.f));
		transform->SetScale(0.1f);
		transform->SetLocalRotation(vec3f(0.f, 90.f, 0.f));

		Light* const light = entities->AddComponent<Light>(entity);
		light->SetActive(true);
		light->intensity = 100.0f;
		light->outerCutOff = 30.f;
		light->range = 100.0f;
		light->type = LIGHT_SPOT;		
		
		MeshRender* const render = entities->AddComponent<MeshRender>(entity);
		render->SetActive(true);
		render->SetMaterial(opaque);
		render->SetMesh(sphere);
	}

	const unsigned pointLight = entities->Create();
	{
		Transform* const transform = entities->GetComponent<Transform>(pointLight);
		transform->SetScale(0.5f);

		Light* const light = entities->AddComponent<Light>(pointLight);
		light->SetActive(true);
		light->SetCastShadows(false);
		light->color = vec4f(253.f / 255.f, 251.f / 255.f, 211.f / 255.f, 1.0f);
		light->intensity = 10.0;
		light->type = LIGHT_POINT;

		MeshRender* const render = entities->AddComponent<MeshRender>(pointLight);
		render->SetActive(true);
		render->SetMaterial(opaque);
		render->SetMesh(sphere);
	}

	Orbit* const orbit = entities->AddComponent<Orbit>(pointLight);
	orbit->SetActive(true);
	orbit->radius = 2.5f;

	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetDynamic(false);

		orbit->orbit = transform;

		MeshRender* const render = entities->AddComponent<MeshRender>(entity);
		render->SetActive(true);
		render->SetMaterial(rustediron);
		render->SetMesh(sphere);
		render->SetDynamic(false);

		AudioSource* const audio = entities->AddComponent<AudioSource>(entity);
		audio->SetActive(true);
		audio->audioClip = "Files/Media/hit.wav";
		audio->volume = 1.f;
		audio->spatialBlend = 1.f;
		audio->Play();

		TestAudio* const test = entities->AddComponent<TestAudio>(entity);
		test->SetActive(true);
	}

	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetScale(vec3f(10.0f, 1.0f, 10.0f));
		transform->SetLocalTranslation(vec3f(0.f, -5.f, 0.f));
		transform->SetDynamic(false);

		MeshRender* const render = entities->AddComponent<MeshRender>(entity);
		render->SetActive(true);
		render->SetMaterial(hardwood);
		render->SetMesh(cube);
		render->SetDynamic(false);
	}

	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetLocalTranslation(vec3f(2.f, -2.f, 0.f));
		transform->SetScale(2.f);
		transform->SetDynamic(false);

		MeshRender* const render = entities->AddComponent<MeshRender>(entity);
		render->SetActive(true);
		render->SetMaterial(window);
		render->SetMesh(cube);
		render->SetDynamic(false);

		BoxCollider* const collider = entities->AddComponent<BoxCollider>(entity);
		collider->SetActive(true);
	}

	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetScale(vec3f(10.0f, 1.0f, 10.0f));
		transform->SetLocalTranslation(vec3f(0.f, 5.f, 0.f));
		transform->SetDynamic(false);

		MeshRender* const render = entities->AddComponent<MeshRender>(entity);
		render->SetActive(true);
		render->SetMaterial(white);
		render->SetMesh(cube);
		render->SetDynamic(false);
	}

	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetScale(vec3f(1.0f, 10.0f, 10.0f));
		transform->SetLocalTranslation(vec3f(-5.f, 0.f, 0.f));
		transform->SetDynamic(false);

		MeshRender* const render = entities->AddComponent<MeshRender>(entity);
		render->SetActive(true);
		render->SetMaterial(white);
		render->SetMesh(cube);
		render->SetDynamic(false);
	}

	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetScale(vec3f(1.0f, 10.0f, 10.0f));
		transform->SetLocalTranslation(vec3f(5.f, 0.f, 0.f));
		transform->SetDynamic(false);

		MeshRender* const render = entities->AddComponent<MeshRender>(entity);
		render->SetActive(true);
		render->SetMaterial(white);
		render->SetMesh(cube);
		render->SetDynamic(false);
	}

	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetScale(vec3f(10.0f, 10.0f, 1.0f));
		transform->SetLocalTranslation(vec3f(0.f, 0.f, -5.f));
		transform->SetDynamic(false);

		MeshRender* const render = entities->AddComponent<MeshRender>(entity);
		render->SetActive(true);
		render->SetMaterial(streakedmetal);
		render->SetMesh(cube);
		render->SetDynamic(false);
	}
}

void SampleScene::Destroy() {
	Scene::Destroy();
	delete opaque;
	delete rustediron;
	delete streakedmetal;
	delete hardwood;
	delete white;
	delete window;
	delete skybox;
}
