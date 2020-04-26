#include "PostProcess.h"

#include "SpriteRenderer.h"

#include <Events/EventsManager.h>
#include <GL/glew.h>	

unsigned PostProcess::VAO = 0;

PostProcess::PostProcess() {
	if (VAO == 0) SpriteRenderer::GenerateQuad(VAO);
}

void PostProcess::SetActive(bool const & state) {
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Trigger("POST_PROCESS_ACTIVE", new Events::AnyType<PostProcess*>(this));
}


