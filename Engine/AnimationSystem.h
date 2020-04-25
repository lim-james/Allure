#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#include "System.h"
#include "Animation.h"

#include <Events/Event.h>

#include <vector>

class AnimationSystem : public System {

	std::vector<Animation*> components;

public:

	void Initialize() override;
	void Update(float const& dt) override;

private:

	// Event handlers
	void ActiveHandler(Events::Event* event);

};

#endif
