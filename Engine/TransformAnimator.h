#ifndef TRANSFORM_ANIMATOR_H
#define TRANSFORM_ANIMATOR_H

#include "Component.h"

#include "AnimationBase.h"
#include "Transform.h"

#include <Math/Vectors.h>
#include <vector>
#include <map>

#define ANIMATE_SCALE		0
#define ANIMATE_ROTATION	1
#define ANIMATE_TRANSLATION 2
#define ANIMATE_COUNT		3

struct TransformAnimator : Component {

	using base_type = TransformAnimator;

	std::map<Transform*, std::vector<TransformAnimationData>> animations[ANIMATE_COUNT];

	~TransformAnimator() override;

	void Initialize() override;
	Component* Clone() const override;
	void SetActive(bool const& state) override;

	void Queue(AnimationBase const& base, Transform* const target, short const& attribute, vec3f const& outcome);

	unsigned Count(Transform* const target, short const& attribute) const;
	void Pop(Transform* const target, short const& attribute);
	void Clear(Transform* const target, short const& attribute);

	void Clear();

};

#endif
