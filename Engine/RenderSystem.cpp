#include "RenderSystem.h"

// ECS
#include "EntityManager.h"
#include "Transform.h"
// renderers
#include "MeshRenderer.h"
#include "SpriteRenderer.h"
#include "LineRenderer.h"
#include "TextRenderer.h"
// Events
#include "InputEvents.h"

#include <Events/EventsManager.h>
#include <Helpers/VectorHelpers.h>
#include <Math/Mat4Transform.hpp>
#include <MACROS.h>
#include <GL/glew.h>

RenderSystem::~RenderSystem() {
	for (Renderer* const r : renderers)
		delete r;

	delete curveShader;
	delete mainFBO;
}

void RenderSystem::Initialize() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	renderers.push_back(new MeshRenderer);
	renderers.push_back(new SpriteRenderer);
	renderers.push_back(new LineRenderer);
	renderers.push_back(new TextRenderer);

	for (Renderer* const r : renderers) {
		r->Initialize(entities);
	}

	curveShader = new Shader("Files/Shaders/fb.vert", "Files/Shaders/curve.frag");
	curveShader->Use();
	curveShader->SetInt("tex", 0);

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

	mainFBO = new Framebuffer(1, 1);
	mainFBO->Initialize(vec2u(1600, 900), { tData }, { rbData });

	Events::EventsManager::GetInstance()->Subscribe("LIGHT_ACTIVE", &RenderSystem::LightActiveHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("CAMERA_ACTIVE", &RenderSystem::CameraActiveHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("CAMERA_DEPTH", &RenderSystem::CameraDepthHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("CAMERA_FRAMEBUFFER", &RenderSystem::CameraFramebufferHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("WINDOW_RESIZE", &RenderSystem::ResizeHandler, this);
}

void RenderSystem::Update(float const& dt) {
	glViewport(0, 0, static_cast<GLsizei>(windowSize.w), static_cast<GLsizei>(windowSize.h));
	glScissor(0, 0, static_cast<GLsizei>(windowSize.w), static_cast<GLsizei>(windowSize.h));
	glClearColor(0, 0, 0, 0);

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

		RendererData data;
		data.camera = cam;
		data.projection = cam->GetProjectionMatrix();
		data.view = entities->GetComponent<Transform>(cam->entity)->GetLocalLookAt();
		data.lights = &lights;

		glViewport(origin.x, origin.y, size.x, size.y);
		glScissor(origin.x, origin.y, size.x, size.y);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glClearColor(0, 0, 0, 0);

		if (cam->shouldClear) {
			glClearColor(cam->clearColor.r, cam->clearColor.g, cam->clearColor.b, cam->clearColor.a);
		}

		for (Renderer* const r : renderers) {
			r->Render(data);
		}

		fb->Unbind();
	}

	for (Camera* const cam : cameras) {
		vec4f const& viewport = cam->GetViewport();

		const Math::vec<2, GLint> origin(
			static_cast<GLint>(viewport.origin.x),
			static_cast<GLint>(viewport.origin.y)
		);

		const Math::vec<2, GLint> size(
			static_cast<GLsizei>(viewport.size.w),
			static_cast<GLsizei>(viewport.size.h)
		);

		RendererData data;
		data.camera = cam;
		data.projection = cam->GetProjectionMatrix();
		data.view = entities->GetComponent<Transform>(cam->entity)->GetLocalLookAt();
		data.lights = &lights;

		glViewport(origin.x, origin.y, size.x, size.y);
		glScissor(origin.x, origin.y, size.x, size.y);
		glClear(GL_DEPTH_BUFFER_BIT);

		if (cam->shouldClear) {
			glClearColor(cam->clearColor.r, cam->clearColor.g, cam->clearColor.b, cam->clearColor.a);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		for (Renderer* const r : renderers) {
			r->Render(data);
		}
	}
}

void RenderSystem::Start() {
	System::Start();

	auto manager = Events::EventsManager::GetInstance();
	for (Renderer* const r : renderers)
		manager->SubscribeContext(r);
}

void RenderSystem::Stop() {
	System::Stop();

	auto manager = Events::EventsManager::GetInstance();
	for (Renderer* const r : renderers)
		manager->UnsubscribeContext(r);
}

void RenderSystem::LightActiveHandler(Events::Event * event) {
	Light* const c = static_cast<Events::AnyType<Light*>*>(event)->data;

	if (c->IsActive()) {
		Helpers::Insert(lights, c);
	} else {
		Helpers::Remove(lights, c);
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

	if (c->GetFramebuffer()) {
		Helpers::Insert(fbCameras, c);
	} else {
		Helpers::Remove(fbCameras, c);
	}
}

void RenderSystem::ResizeHandler(Events::Event* event) {
	windowSize = static_cast<Events::AnyType<vec2i>*>(event)->data;
	mainFBO->Resize(windowSize);
}
