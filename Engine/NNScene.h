#ifndef NN_SCENE_H
#define NN_SCENE_H

#include "Scene.h"

#include "Physics.h"

#include <Events/Event.h>

#include <vector>

class NNScene : public Scene {

	float vertSpeed;
	float gameSpeed;
	Physics * birdPhysics;

	std::vector<std::pair<unsigned, unsigned>> pipes;

public:

	void Awake() override;
	void Update(const float& dt) override;

private:

	void KeyHandler(Events::Event* event);

	void CollisionHandler(unsigned target);

	void CreatePipe(const float& x, const float& openingY);

	void ResetGame();

};

#endif
