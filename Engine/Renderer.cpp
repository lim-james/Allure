#include "Renderer.h"

Renderer::~Renderer() {}

void Renderer::Initialize(EntityManager * const manager) {
	entities = manager;
}
