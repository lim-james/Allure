#ifndef CAMERA_H
#define CAMERA_H

#include "Component.h"
#include "Framebuffer.h"

#include <Events/Event.h>
#include <Math/Vectors.h>
#include <Math/Mat4.hpp>
#include <Bit/BitField.h>

#define PERSPECTIVE		0	
#define ORTHOGRAPHIC	1	

struct Camera : Component {

	using base_type = Camera;

	bool isHidden;

	bool shouldClear;
	vec4f clearColor;

	short projection;

	float FOV;

	float nearPlane;
	float farPlane;

	bool captureDepth;

	BitField cullingMask;

	Camera();
	~Camera() override;

	void Initialize() override;
	Component* Clone() const override;

	void SetActive(bool const& state) override;

	float const& GetSize() const;
	void SetSize(float const& value);
	void SetMatch(float const& value);

	float const& GetDepth() const;
	void SetDepth(float const& value);

	mat4f GetProjectionMatrix() const;

	vec2f const& GetFrameSize() const;

	void SetViewportRect(vec4f const& rect);
	vec4f const& GetViewport() const;

	void SetFramebuffer(Framebuffer* const fb);
	Framebuffer* const GetFramebuffer() const;

	void SetDepthBuffer(Framebuffer* const fb);
	Framebuffer* const GetDepthBuffer() const;

	vec2f ScreenToWorldPosition(vec2f const& mousePosition) const;

	bool UseProcess() const;
	void SetUseProcess(bool const& state);

private:

	float size;
	float match;

	float depth;
	vec4f viewportRect;

	float aspectRatio;
	float left, right, bottom, top;
	vec2f frameSize;
	vec4f viewport;

	vec2f windowSize;
	Framebuffer* framebuffer;
	Framebuffer* depthBuffer;

	bool useProcess;

	void WindowResizeHandler(Events::Event* event);

	void UpdateViewport();

};


#endif