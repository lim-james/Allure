#include "Camera.h"

#include "Layers.h"

#include <Events/EventsManager.h>
#include <Math/Mat4Transform.hpp>

Camera::Camera()
	: shouldClear(true)
	, clearColor(0.f)

	, projection(PERSPECTIVE)

	, FOV(60.0f)

	, nearPlane(0.1f)
	, farPlane(100.0f)

	, cullingMask(DEFAULT | UI)

	, size(5.f)
	, match(0.f)

	, depth(0.0f)
	, viewportRect(0.0f, 0.0f, 1.0f, 1.0f)

	, aspectRatio(1.f)
	, left(0.f)
	, right(1.f)
	, bottom(0.f)
	, top(1.f)
	, viewport(vec2f(0.f), vec2f(1.f))

	, windowSize(1.f)
	, framebuffer(nullptr) {

	Events::EventsManager::GetInstance()->Subscribe("WINDOW_RESIZE", &Camera::WindowResizeHandler, this);
}

Camera::~Camera() {}

void Camera::Initialize() {
	shouldClear = true;
	clearColor = vec4f(0.f);

	projection = PERSPECTIVE;

	FOV = 60.0f;

	nearPlane = 0.1f;
	farPlane = 100.0f;

	cullingMask.Set(DEFAULT | UI);

	size = 5.f;
	match = 0.f;

	depth = 0.0f;
	viewportRect = vec4f(vec2f(0.f), vec2f(1.f));

	vec2i size;
	Events::EventsManager::GetInstance()->Trigger("GET_WINDOW_SIZE", new Events::AnyType<vec2i*>(&size));
	windowSize = size;
	framebuffer = nullptr;

	UpdateViewport();
}

Component * Camera::Clone() const {
	return new Camera(*this);
}

void Camera::SetActive(bool const& state) {
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Trigger("CAMERA_ACTIVE", new Events::AnyType<Camera*>(this));
}

float const& Camera::GetSize() const {
	return size;
}

void Camera::SetSize(float const& value) {
	size = value;
	UpdateViewport();
}

float const& Camera::GetDepth() const {
	return depth;
}

void Camera::SetDepth(float const& value) {
	depth = value;
	Events::EventsManager::GetInstance()->Trigger("CAMERA_DEPTH", new Events::AnyType<Camera*>(this));
}

mat4f Camera::GetProjectionMatrix() const {
	mat4f result;
	if (projection == PERSPECTIVE) {
		result = Math::Perspective(FOV, aspectRatio, nearPlane, farPlane);
	} else {
		result = Math::Orthographic(left, right, bottom, top, nearPlane, farPlane);
	}

	return result;
}

void Camera::SetMatch(float const& value) {
	match = value;
	UpdateViewport();
}

void Camera::SetViewportRect(vec4f const& rect) {
	viewportRect = rect;
	UpdateViewport();
}

vec4f const& Camera::GetViewport() const {
	return viewport;
}

void Camera::SetFramebuffer(Framebuffer * const fb) {
	framebuffer = fb;
	Events::EventsManager::GetInstance()->Trigger("CAMERA_FRAMEBUFFER", new Events::AnyType<Camera*>(this));
}

Framebuffer * const Camera::GetFramebuffer() const {
	return framebuffer;
}

vec2f Camera::ScreenToWorldSpace(vec2f const& mousePosition) const {
	const vec2f viewportPosition = mousePosition - viewport.origin;

	vec2f unitPosition = viewportPosition / viewport.size;
	unitPosition = unitPosition * 2.f - vec2f(1.f);

	vec2f result = unitPosition * size;
	result.x *= aspectRatio;
	//result.y = -result.y;

	return result;
}

void Camera::WindowResizeHandler(Events::Event* event) {
	windowSize = static_cast<Events::AnyType<vec2i>*>(event)->data;
	UpdateViewport();
}

void Camera::UpdateViewport() {
	viewport = vec4f(windowSize, windowSize) * viewportRect;

	const float invMatch = 1.f - match;
	aspectRatio = (viewport.size.w * invMatch + viewport.size.h * match) / (viewport.size.w * match + viewport.size.h * invMatch);

	const float unit = aspectRatio * size;
	const float w = unit * invMatch + size * match;
	const float h = unit * match + size * invMatch;
	left = -w, right = w;
	bottom = -h, top = h;

	if (framebuffer) framebuffer->Resize(viewport.size);
}