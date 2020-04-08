#include "RenderSystem.h"

#include "EntityManager.h"
#include "Transform.h"

#include "InputEvents.h"

#include <Events/EventsManager.h>
#include <Helpers/VectorHelpers.h>
#include <Math/Mat4Transform.hpp>
#include <MACROS.h>
#include <GL/glew.h>

bool operator==(Instance const& lhs, Instance const& rhs) {
	return lhs.component == rhs.component;
}

RenderSystem::~RenderSystem() {
	for (auto& r : renderers)
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

	renderers.push_back(new SpriteRenderer);
	renderers.push_back(new LineRenderer);
	renderers.push_back(new TextRenderer);

	for (auto& r : renderers) {
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

	Events::EventsManager::GetInstance()->Subscribe("CAMERA_ACTIVE", &RenderSystem::CameraActiveHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("CAMERA_DEPTH", &RenderSystem::CameraDepthHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("WINDOW_RESIZE", &RenderSystem::ResizeHandler, this);
}

void RenderSystem::Update(float const& dt) {
	glViewport(0, 0, static_cast<GLsizei>(windowSize.w), static_cast<GLsizei>(windowSize.h));
	glScissor(0, 0, static_cast<GLsizei>(windowSize.w), static_cast<GLsizei>(windowSize.h));
	glClearColor(0, 0, 0, 0);

	for (auto& cam : cameras) {
		auto const& viewport = cam->GetViewport();

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

		glViewport(origin.x, origin.y, size.x, size.y);
		glScissor(origin.x, origin.y, size.x, size.y);
		glClearColor(cam->clearColor.r, cam->clearColor.g, cam->clearColor.b, cam->clearColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (auto& r : renderers) {
			r->Render(data);
		}
	}
}

void RenderSystem::Start() {
	System::Start();

	auto manager = Events::EventsManager::GetInstance();
	for (auto& r : renderers)
		manager->SubscribeContext(r);
}

void RenderSystem::Stop() {
	System::Stop();

	auto manager = Events::EventsManager::GetInstance();
	for (auto& r : renderers)
		manager->UnsubscribeContext(r);
}

void RenderSystem::CameraActiveHandler(Events::Event* event) {
	auto& c = static_cast<Events::AnyType<Camera*>*>(event)->data;

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
	auto& c = static_cast<Events::AnyType<Camera*>*>(event)->data;

	if (c->IsActive()) {
		cameras.erase(vfind(cameras, c));
		for (unsigned i = 0; i < cameras.size(); ++i) {
			if (cameras[i]->GetDepth() >= c->GetDepth()) {
				cameras.insert(cameras.begin() + i, c);
				return;
			}
		}
		cameras.push_back(c);
	}
}

void RenderSystem::ResizeHandler(Events::Event* event) {
	windowSize = static_cast<Events::AnyType<vec2i>*>(event)->data;
	mainFBO->Resize(windowSize);
}
