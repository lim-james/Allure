#include "LineRenderer.h"

#include "Transform.h"

#include <Events/EventsManager.h>
#include <GL/glew.h>

unsigned LineRenderer::instanceBuffer = 0;
unsigned LineRenderer::lineVAO = 0;

LineRenderer::~LineRenderer() {
	delete shader;
}

void LineRenderer::Initialize(EntityManager * const manager) {
	Renderer::Initialize(manager);

	if (lineVAO == 0)
		GenerateLine();

	shader = new Shader("Files/Shaders/line.vert", "Files/Shaders/standard2D.frag");
	shader->Use();
	shader->SetInt("tex", 0);

	Events::EventsManager::GetInstance()->Subscribe("DRAW_LINE", &LineRenderer::DrawLineHandler, this);
}

void LineRenderer::Render(const RendererData& data) {
	glDisable(GL_DEPTH_TEST);

	const unsigned count = lines.size();
	if (count) {
		glLineWidth(data.camera->GetSize() * .25f);

		glBindVertexArray(lineVAO);
		shader->Use();
		shader->SetMatrix4("projection", data.projection);
		shader->SetMatrix4("view", data.view);

		const unsigned stride = sizeof(Line);

		glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
		glBufferData(GL_ARRAY_BUFFER, count * stride, &lines[0], GL_STATIC_DRAW);

		// offset	
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, 0);
		glVertexAttribDivisor(1, 1);
		// length	
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(vec2f));
		glVertexAttribDivisor(2, 1);
		// tint	
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(vec4f));
		glVertexAttribDivisor(3, 1);

		shader->SetInt("useTex", 0);
		glDrawArraysInstanced(GL_LINES, 0, 2, count);

		lines.clear();
	}

	glEnable(GL_DEPTH_TEST);
}

void LineRenderer::DrawLineHandler(Events::Event* event) {
	lines.push_back(
		static_cast<Events::AnyType<Line>*>(event)->data
	);
}

void LineRenderer::GenerateLine() {
	float lineVertices[] = {
		0.f, 0.f,
		1.f, 1.f
	};

	unsigned VBO;
	glGenVertexArrays(1, &lineVAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(lineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), &lineVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vec2f), (void*)0);
}

