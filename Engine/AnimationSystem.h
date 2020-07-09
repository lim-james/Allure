#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#include "System.h"
#include "Animator.h"
#include "TransformAnimator.h"

#include "Transform.h"

#include <Events/Event.h>

#include <vector>

class AnimationSystem : public System {

	std::vector<Animator*> base;
	std::vector<TransformAnimator*> transforms;

public:

	void Initialize() override;
	void Update(float const& dt) override;

private:

	// Event handlers
	void BaseActiveHandler(Events::Event* event);
	void TransformActiveHandler(Events::Event* event);

	// transform handlers
	void UpdateTransform(
		std::vector<TransformAnimationData>& animations, 
		Transform* const transform, 
		vec3f const& (Transform::*getter)() const, 
		void(Transform::*setter)(vec3f const&), 
		float const& dt
	);

};

#endif
