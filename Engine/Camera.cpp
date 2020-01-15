#include "Camera.h"

#include <Events/EventsManager.h>
#include <Math/MatrixTransform.hpp>

Camera::Camera()
	: clearColor(0.f)

	, size(5.f)

	, nearPlane(0.1f)
	, farPlane(100.0f)

	, depth(0.0f)
	, viewportRect(0.0f, 0.0f, 1.0f, 1.0f)

	, aspectRatio(1.f)
	, left(0.f)
	, right(1.f)
	, bottom(0.f)
	, top(1.f)
	, viewport(vec2f(0.f), vec2f(1.f))

	, windowSize(1.f) {

	Events::EventsManager::GetInstance()->Subscribe("WINDOW_RESIZE", &Camera::WindowResizeHandler, this);
}

Camera::~Camera() {}

void Camera::Initialize() {
	clearColor.Set(0.f);

	size = 5.f;

	nearPlane = 0.1f;
	farPlane = 100.0f;

	depth = 0.0f;
	viewportRect.Set(vec2f(0.f), vec2f(1.f));

	vec2i size;
	Events::EventsManager::GetInstance()->Trigger("GET_WINDOW_SIZE", new Events::AnyType<vec2i*>(&size));
	windowSize = size;

	UpdateViewport();
}

void Camera::SetActive(const bool& state) {
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Trigger("CAMERA_ACTIVE", new Events::AnyType<Camera*>(this));
}

void Camera::SetSize(const float& value) {
	size = value;
	UpdateViewport();
}

void Camera::SetDepth(const float& value) {
	depth = value;
	Events::EventsManager::GetInstance()->Trigger("CAMERA_DEPTH", new Events::AnyType<Camera*>(this));
}

mat4f Camera::GetProjectionMatrix() const {
	return Math::Orthographic(left, right, bottom, top, nearPlane, farPlane);
}

void Camera::SetViewportRect(const vec4f& rect) {
	viewportRect = rect;
	UpdateViewport();
}

const vec4f& Camera::GetViewport() const {
	return viewport;
}

vec2f Camera::ScreenToWorldSpace(const vec2f & mousePosition) const {
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
	aspectRatio = viewport.size.w / viewport.size.h;
	const float w = aspectRatio * size;
	left = -w, right = w;
	bottom = -size, top = size;
}