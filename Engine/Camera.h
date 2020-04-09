#ifndef CAMERA_H
#define CAMERA_H

#include "Component.h"

#include <Events/Event.h>
#include <Math/Vectors.h>
#include <Math/Mat4.hpp>
#include <Bit/BitField.h>

struct Camera : Component {

	bool shouldClear;
	vec4f clearColor;

	float nearPlane;
	float farPlane;

	BitField cullingMask;

	Camera();
	~Camera();

	void Initialize() override;
	Component* Clone() const override;

	void SetActive(bool const& state) override;

	float const& GetSize() const;
	void SetSize(float const& value);
	void SetMatch(float const& value);

	float const& GetDepth() const;
	void SetDepth(float const& value);

	mat4f GetProjectionMatrix() const;

	void SetViewportRect(vec4f const& rect);
	vec4f const& GetViewport() const;

	vec2f ScreenToWorldSpace(vec2f const& mousePosition) const;

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