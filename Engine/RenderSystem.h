#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "System.h"
#include "Render.h"
#include "Camera.h"
#include "Text.h"
#include "Shader.h"
#include "Line.h"
#include "Framebuffer.h"

#include <Events/Event.h>

#include <vector>
#include <map>

struct Instance {
	Render* component;
	vec4f uvRect;
	vec4f tint;
	mat4f model;
};

bool operator==(const Instance& lhs, const Instance& rhs);

typedef std::map<unsigned, std::vector<Instance>> Batches;

class RenderSystem : public System {

	bool debugging;

	std::vector<Camera*> cameras;

	std::vector<Line> lines;
	std::map<Font*, std::vector<Text*>> textBatches;

	static const unsigned INSTANCE_LAYOUT_LOCATION = 2;
	static unsigned instanceBuffer;

	static unsigned quadVAO;
	static unsigned lineVAO;

	Shader* mainShader;
	Shader* lineShader;
	Shader* textShader;

	Shader* curveShader;
	Framebuffer* mainFBO;

	Batches batches;

	vec2f windowSize;

public:

	~RenderSystem() override;

	void Initialize() override;
	void Update(const float& dt) override;

private:

	void KeyHandler(Events::Event* event);

	void CameraActiveHandler(Events::Event* event);
	void CameraDepthHandler(Events::Event* event);

	void RenderActiveHandler(Events::Event* event);
	void TextureChangeHandler(Events::Event* event);
	void DrawLineHandler(Events::Event* event);

	void TextActiveHandler(Events::Event* event);
	void TextFontHandler(Events::Event* event);

	void ResizeHandler(Events::Event* event);

	void GenerateQuad();
	void GenerateLine();

};

#endif
