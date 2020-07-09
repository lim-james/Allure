#ifndef TRANSFORM_ANIMATOR_H
#define TRANSFORM_ANIMATOR_H

#include "Component.h"

#include "AnimationBase.h"

#include <Math/Vectors.h>
#include <vector>

#define ANIMATE_SCALE		0
#define ANIMATE_ROTATION	1
#define ANIMATE_TRANSLATION 2
#define ANIMATE_COUNT		3



struct TransformAnimator : Component {

	using base_type = TransformAnimator;

	std::vector<TransformAnimationData> animations[ANIMATE_COUNT];

	~TransformAnimator() override;

	void Initialize() override;
	Component* Clone() const override;
	void SetActive(bool const& state) override;

	void Queue(AnimationBase const& base, short const& target, vec3f const& outcome);

	unsigned Count(short const& target) const;
	void Pop(short const& target);
	void Clear(short const& target);

	void Clear();

};

#endif
