#ifndef ANIMATION_BASE_H
#define ANIMATION_BASE_H

#include <Handler/Handler.h>
#include <Math/Vectors.h>

class AnimationBase {

protected:

	bool loop;
	float duration;
	float delay;

	Handler<void, void> completion;

	float et;

public:

	AnimationBase(
		bool const& loop = false,
		float const& duration = 0.f,
		float const& delay = 0.f,
		Handler<void, void> completion = nullptr);

	virtual ~AnimationBase() {}

	virtual void Update(float const& dt);
	const bool IsActive() const;

	float const& GetDuration() const;
	float const& GetDelay() const;

	friend class AnimationSystem;

};

template<typename T>
class AnimationData : public AnimationBase {

private:

	T* target;
	T outcome;
	T original;

public:

	AnimationData() {}
	~AnimationData() override {}

	AnimationData(AnimationBase const& base, T* target, T const& outcome)
		: AnimationBase(base)
		, target(target)
		, outcome(outcome)
		, original(*target) {}


	void Update(float const& dt) override {
		AnimationBase::Update(dt);

		if (et >= duration) {
			if (loop) {
				*target = original;
				et = 0.f;
			} else {
				*target = outcome;
				completion.Invoke();
			}
		} else if (et > 0.f) {
			const T interval = outcome - *(target);
			const float ratio = dt / (duration - et);
			*(target) += interval * ratio;
		}
	}
};

class TransformAnimationData : public AnimationBase {

public:

	vec3f original;
	vec3f outcome;

	TransformAnimationData(AnimationBase base, vec3f const& original, vec3f const& outcome)
		: AnimationBase(base) 
		, original(original)
		, outcome(outcome) {}

};

#endif
