#include "RenderSystem.h"

// ECS
#include "EntityManager.h"
#include "Transform.h"
// renderers
#include "MeshRenderer.h"
#include "VoxelRenderer.h"
#include "SpriteRenderer.h"
#include "LineRenderer.h"
#include "TextRenderer.h"

#include <Events/EventsManager.h>
#include <Helpers/VectorHelpers.h>
#include <Math/Mat4Transform.hpp>
#include <MACROS.h>
#include <GL/glew.h>

unsigned RenderSystem::VAO = 0;

RenderSystem::~RenderSystem() {
	for (Framebuffer* const fb : depthFBO)
		delete fb;

	for (Renderer* const r : renderers)
		delete r;

	delete mainFBO;
	delete fbShader;
	delete postProccessing;
}

void RenderSystem::Initialize() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	{
		TextureData tData;
		tData.level = 0;
		tData.internalFormat = GL_DEPTH_COMPONENT;
		tData.border = 0;
		tData.format = GL_DEPTH_COMPONENT;
		tData.type = GL_FLOAT;
		tData.attachment = GL_DEPTH_ATTACHMENT;
		tData.parameters.push_back({ GL_TEXTURE_MIN_FILTER, GL_LINEAR });
		tData.parameters.push_back({ GL_TEXTURE_MAG_FILTER, GL_LINEAR });
		tData.parameters.push_back({ GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER });
		tData.parameters.push_back({ GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER });

		for (unsigned i = 0; i < MAX_LIGHTS; ++i) {
			depthFBO[i] = new Framebuffer(1, 0);
			depthFBO[i]->Initialize(vec2u(1000, 1000), { tData }, { });
		}
	}

	renderers.push_back(new MeshRenderer);
	renderers.push_back(new VoxelRenderer);
	renderers.push_back(new SpriteRenderer);
	renderers.push_back(new LineRenderer);
	renderers.push_back(new TextRenderer);

	for (Renderer* const r : renderers)
		r->Initialize(entities);

	postProccessing = new PostProcessStack;
	postProccessing->rawRender.Bind(&RenderSystem::Render, this);

	{
		std::vector<TextureData> tDataList;

		for (int i = 0; i < 2; ++i) {
			TextureData tData;
			tData.level = 0;
			tData.internalFormat = GL_RGB16F;
			tData.border = 0;
			tData.format = GL_RGBA;
			tData.type = GL_UNSIGNED_BYTE;
			tData.attachment = GL_COLOR_ATTACHMENT0 + i;
			tData.parameters.push_back({ GL_TEXTURE_MIN_FILTER, GL_LINEAR });
			tData.parameters.push_back({ GL_TEXTURE_MAG_FILTER, GL_LINEAR });
			tData.parameters.push_back({ GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE });
			tData.parameters.push_back({ GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE });
			tDataList.push_back(tData);
		}

		RenderBufferData rbData;
		rbData.internalFormat = GL_DEPTH24_STENCIL8;
		rbData.attachmentFormat = GL_DEPTH_STENCIL_ATTACHMENT;

		mainFBO = new Framebuffer(2, 1);
		mainFBO->Initialize(vec2u(RESOLUTION * 16.0 / 9.0, RESOLUTION), tDataList, { rbData });
	}

	fbShader = new Shader("Files/Shaders/fb.vert", "Files/Shaders/upscale.frag");
	fbShader->Use();
	fbShader->SetInt("fragTex", 0);
	fbShader->SetInt("brightTex", 1);

	if (VAO == 0) SpriteRenderer::GenerateQuad(VAO);

	EventsManager* const em = EventsManager::Get();
	em->Subscribe("LIGHT_ACTIVE", &RenderSystem::LightActiveHandler, this);
	em->Subscribe("LIGHT_CAST_SHADOWS", &RenderSystem::LightShadowHanlder, this);
	em->Subscribe("CAMERA_ACTIVE", &RenderSystem::CameraActiveHandler, this);
	em->Subscribe("CAMERA_DEPTH", &RenderSystem::CameraDepthHandler, this);
	em->Subscribe("CAMERA_FRAMEBUFFER", &RenderSystem::CameraFramebufferHandler, this);
	em->Subscribe("WINDOW_RESIZE", &RenderSystem::ResizeHandler, this);
}

void RenderSystem::Update(float const& dt) {
	DepthRender();
	FBRender();
	RenderLowRes();
	postProccessing->Render();
}

void RenderSystem::Start() {
	System::Start();

	auto manager = EventsManager::Get();
	for (Renderer* const r : renderers)
		manager->SubscribeContext(r);
	manager->SubscribeContext(postProccessing);
}

void RenderSystem::Stop() {
	System::Stop();

	auto manager = EventsManager::Get();
	for (Renderer* const r : renderers)
		manager->UnsubscribeContext(r);
	manager->UnsubscribeContext(postProccessing);
}

void RenderSystem::LightActiveHandler(Events::Event * event) {
	Light* const c = static_cast<Events::AnyType<Light*>*>(event)->data;

	if (c->IsActive()) {
		if (Helpers::Insert(lights, c)) {
			casters.push_back(c);
		}
	} else {
		if (Helpers::Remove(lights, c)) {
			Helpers::Remove(casters, c);
		}
	}
}

void RenderSystem::LightShadowHanlder(Events::Event * event) {
	Light* const c = static_cast<Events::AnyType<Light*>*>(event)->data;

	if (!c->IsActive()) return;

	if (c->CastShadows()) {
		Helpers::Insert(casters, c);
	} else {
		Helpers::Remove(casters, c);
	}
}

void RenderSystem::CameraActiveHandler(Events::Event* event) {
	Camera* const c = static_cast<Events::AnyType<Camera*>*>(event)->data;

	if (c->IsActive()) {
		for (unsigned i = 0; i < cameras.size(); ++i) {
			if (cameras[i] == c) {
				return;
			} else if (cameras[i]->GetDepth() >= c->GetDepth()) {
				cameras.insert(cameras.begin() + i, c);
				return;
			}
		}
		cameras.push_back(c);
	} else {
		Helpers::Remove(cameras, c);
	}
}

void RenderSystem::CameraDepthHandler(Events::Event* event) {
	Camera* const c = static_cast<Events::AnyType<Camera*>*>(event)->data;

	if (c->IsActive()) {
		Helpers::Remove(cameras, c);
		for (unsigned i = 0; i < cameras.size(); ++i) {
			if (cameras[i]->GetDepth() >= c->GetDepth()) {
				cameras.insert(cameras.begin() + i, c);
				return;
			}
		}
		cameras.push_back(c);
	}
}

void RenderSystem::CameraFramebufferHandler(Events::Event * event) {
	Camera* const c = static_cast<Events::AnyType<Camera*>*>(event)->data;

	if (!c->IsActive()) return;

	if (c->GetFramebuffer()) {
		Helpers::Insert(fbCameras, c);
	} else {
		Helpers::Remove(fbCameras, c);
	}
}

void RenderSystem::ResizeHandler(Events::Event * event) {
	windowSize = static_cast<Events::AnyType<vec2i>*>(event)->data;
	scaleFactor = static_cast<float>(RESOLUTION) / static_cast<float>(windowSize.h);
	const float ratio = static_cast<float>(windowSize.w) / static_cast<float>(windowSize.h);
	const vec2u newResolution = vec2u(RESOLUTION * ratio, RESOLUTION);
	mainFBO->Resize(newResolution);

	EventsManager::Get()->Trigger("RESOLUTION_CHANGE", new Events::AnyType<vec2u>(newResolution));
}

void RenderSystem::DepthRender() {
	glCullFace(GL_FRONT);
	glClearColor(0, 0, 0, 0);
	for (unsigned i = 0; i < casters.size(); ++i) {
		Light* const light = casters[i];
		Framebuffer* const fb = depthFBO[i];

		const vec2u size = fb->GetSize();
		glViewport(0, 0, size.w, size.h);
		glScissor(0, 0, size.w, size.h);

		Transform* const transform = entities->GetComponent<Transform>(light->entity);

		RendererData data;
		data.object = (void*)light;
		data.projection = light->GetProjectionMatrix();
		data.view = transform->GetWorldLookAt();
		data.cullingMask = light->cullingMask;
		data.lights = nullptr;

		fb->Bind();

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		for (Renderer* const r : renderers)
			r->RenderDepth(data);

		fb->Unbind();

		light->shadowMap = fb->GetTexture();
		lightSpaceMatrices[i] = data.projection * data.view;
	}
	glCullFace(GL_BACK);
}

void RenderSystem::FBRender() {
	for (Camera* const cam : fbCameras) {
		Framebuffer* const fb = cam->GetFramebuffer();
		fb->Bind();

		vec4f const& viewport = cam->GetViewport();

		const Math::vec<2, GLint> origin(
			static_cast<GLint>(viewport.origin.x),
			static_cast<GLint>(viewport.origin.y)
		);

		const Math::vec<2, GLint> size(
			static_cast<GLsizei>(viewport.size.w),
			static_cast<GLsizei>(viewport.size.h)
		);

		Transform* const transform = entities->GetComponent<Transform>(cam->entity);

		RendererData data;
		data.projection = cam->GetProjectionMatrix();
		data.view = transform->GetWorldLookAt();
		data.object = (void*)cam;
		data.viewPosition = transform->GetWorldTranslation();
		data.cullingMask = cam->cullingMask;
		data.lights = &lights;
		data.lightSpaceMatrices = lightSpaceMatrices;

		glViewport(origin.x, origin.y, size.x, size.y);
		glScissor(origin.x, origin.y, size.x, size.y);
		glClearColor(cam->clearColor.r, cam->clearColor.g, cam->clearColor.b, cam->clearColor.a);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		Render(data);

		fb->Unbind();
	}
}

void RenderSystem::Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	fbShader->Use();
	glBindVertexArray(VAO);
	// frag color
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mainFBO->GetTexture());
	// bright color
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mainFBO->GetTexture(1));
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void RenderSystem::RenderLowRes() {
	mainFBO->Bind();

	for (Renderer* const r : renderers) {
		r->PreRender();
	}

	for (Camera* const cam : cameras) {
		if (cam->isHidden) continue;

		Transform* const transform = entities->GetComponent<Transform>(cam->entity);

		RendererData data;
		data.projection = cam->GetProjectionMatrix();
		data.view = transform->GetWorldLookAt();
		data.object = (void*)cam;
		data.viewPosition = transform->GetWorldTranslation();
		data.cullingMask = cam->cullingMask;
		data.lights = &lights;
		data.lightSpaceMatrices = lightSpaceMatrices;

		const vec4i viewport = cam->GetViewport() * scaleFactor;
		glViewport(viewport.origin.x, viewport.origin.y, viewport.size.x, viewport.size.y);
		glScissor(viewport.origin.x, viewport.origin.y, viewport.size.x, viewport.size.y);

		if (cam->shouldClear) {
			glClearColor(cam->clearColor.r, cam->clearColor.g, cam->clearColor.b, cam->clearColor.a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		} else {
			glClear(GL_DEPTH_BUFFER_BIT);
		}

		Render(data);
	}

	for (Renderer* const r : renderers) {
		r->PostRender();
	}

	mainFBO->Unbind();
}

void RenderSystem::Render(RendererData const & data) {
	for (Renderer* const r : renderers) 
		r->RenderOpaque(data);

	for (Renderer* const r : renderers)
		r->RenderTransparent(data);
}
