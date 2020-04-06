#ifndef CAMERA_H
#define CAMERA_H

#include "Component.h"

#include <Events/Event.h>
#include <Math/Vectors.h>
#include <Math/Mat4.hpp>

struct Camera : Component {

	vec4f clearColor;

	float nearPlane;
	float farPlane;

	Camera();
	~Camera();

	void Initialize() override;
	Component* Clone() const override;

	void SetActive(const bool& state) override;

	const float& GetSize() const;
	void SetSize(const float& value);
	void SetMatch(const float& value);

	const float& GetDepth() const;
	void SetDepth(const float& value);

	mat4f GetProjectionMatrix() const;

	void SetViewportRect(const vec4f& rect);
	const vec4f& GetViewport() const;

	vec2f ScreenToWorldSpace(const vec2f& mousePosition) const;

private:

	float size;
	float match;

	float depth;
	vec4f viewportRect;

	float aspectRatio;
	float left, right, bottom, top;
	vec4f viewport;

	vec2f windowSize;

	void WindowResizeHandler(Events::Event* event);

	void UpdateViewport();

};


#endif